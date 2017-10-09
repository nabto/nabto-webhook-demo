/**
 *  Implementation of main for uNabto SDK
 */
#include <sys/types.h>
#include <modules/cli/gopt/gopt.h> // http://www.purposeful.co.uk/software/gopt/
#include <modules/util/read_hex.h>
#include "unabto/unabto_env_base.h"
#include "unabto/unabto_common_main.h"
#include "unabto/unabto_logging.h"
#include "demo_application.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define STDIN 0

struct configuration {
    const char *device_id;
    const char *pre_shared_key;
    const char *token;
    bool send_one_quit;
};
    
void nabto_yield(int msec);
static void help(const char* errmsg, const char *progname);
bool parse_argv(int argc, char* argv[], struct configuration* config, nabto_main_setup* nms);

static void help(const char* errmsg, const char *progname)
{
    if (errmsg) {
        printf("ERROR: %s\n", errmsg);
    }
    printf("\nWebhook notification demo stub application to show how to use webhook notifications.\n");
    printf("Obtain a device id and crypto key from www.appmyproduct.com\n\n");

    printf("Usage: %s -d <device id> -k <crypto key> \n\n", progname);
}


int main(int argc, char* argv[])
{
    struct configuration config;
    memset(&config, 0, sizeof(struct configuration));
    nabto_main_setup* nms = unabto_init_context();

    // Parse command line arguments
    if (!parse_argv(argc, argv, &config, nms)) {
        help(0, argv[0]);
        return 1;
    }

    // configure the Nabto main setup 
    nms->id = strdup(config.device_id);
    nms->secureAttach = true;
    nms->secureData = true;
    nms->cryptoSuite = CRYPT_W_AES_CBC_HMAC_SHA256;

    if (!unabto_read_psk_from_hex(config.pre_shared_key, nms->presharedKey, 16)) {
        help("Invalid cryptographic key specified", argv[0]);
        return false;
    }

    // Initialize uNabto and the demo application
    if (!unabto_init()) {
        NABTO_LOG_FATAL(("Failed at nabto_main_init"));
    }
    demo_init();

    NABTO_LOG_INFO(("Webhook device demo [%s] running!", nms->id));
    fd_set fds;
    int maxfd = STDIN;
    struct timeval tv;
    char buf[5];

    // Run infinite loop which ticks uNabto every 10ms.
    // Also checks for keyboard input to trigger a notification
    while (true) {
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        unabto_tick();
        FD_ZERO(&fds);
        FD_SET(STDIN, &fds); 

        int retval = select(maxfd+1, &fds, NULL, NULL, &tv); 

        // If keyboard is pressed, a notification is send
        if (FD_ISSET(STDIN, &fds)){
            read(STDIN_FILENO, buf, 5);
            NABTO_LOG_INFO(("User triggered a push notification"));
            sendPN();
        }
    }

    unabto_close();
    return 0;
}

bool parse_argv(int argc, char* argv[], struct configuration* config, nabto_main_setup* nms) {
    const char x0s[] = "h?";     const char* x0l[] = { "help", 0 };
    const char x1s[] = "d";      const char* x1l[] = { "deviceid", 0 };
    const char x2s[] = "k";      const char* x2l[] = { "presharedkey", 0 };
    
    const struct { int k; int f; const char *s; const char*const* l; } opts[] = {
        { 'h', 0,           x0s, x0l },
        { 'd', GOPT_ARG,    x1s, x1l },
        { 'k', GOPT_ARG,    x2s, x2l },
        { 0, 0, 0, 0 }
    };
    void *options = gopt_sort( & argc, (const char**)argv, opts);
    
    if( gopt( options, 'h')) {
        help(0, argv[0]);
        exit(0);
    }

    if (!gopt_arg(options, 'd', &config->device_id)) {
        return false;
    }

    if (!gopt_arg(options, 'k', &config->pre_shared_key)) {
        return false;
    }

    return true;
}

