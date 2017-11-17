/**
 *  uNabto application logic implementation
 */
#include "demo_application.h"
#include "unabto/unabto_app.h"
#include "unabto/unabto_util.h"
#include "unabto/unabto_protocol_defines.h"
#include <modules/push_service/push_service.h>
#include <stdio.h>

/* Data to send to the webhook. The staticData of the push_message structure must be JSON formatted.
 * This example uses IFTTT (www.ifttt.com), where the webhook service only supports JSON values named
 * "value1", "value2", and "value3". However, Nabto supports any valid JSON document.
 */
const char staticData[] = {"{\"value1\": \"Temperature Warning:\",\"value2\": \"943\",\"value3\": \"degrees\"}"};

int context = 594;
    
/* Callback function called when the core is done handling a push notification.
 * @param ptr   void pointer to some context passed to the core
 * @param hint  Hint to the success or failure of the push notification.
 */
void callback(void* ptr, const unabto_push_hint* hint){
    switch(*hint){
        case UNABTO_PUSH_HINT_OK:
            // The Push notification was handled successfully by the basestation
            NABTO_LOG_INFO(("Callback with hint: OK"));
            break;
        case UNABTO_PUSH_HINT_QUEUE_FULL:
            // The uNabto core queue has no room for more push notifications
            NABTO_LOG_INFO(("Callback with hint: QUEUE FULL"));
            break;
        case UNABTO_PUSH_HINT_INVALID_DATA_PROVIDED:
            // The uNabto core cannot handle the data provided, probably too large for its communication buffer
            NABTO_LOG_INFO(("Callback with hint: INVALID DATA PROVIDED"));
            break;
        case UNABTO_PUSH_HINT_NO_CRYPTO_CONTEXT:
            // uNabto core cannot send the notification as no cryptographic context exists, are you attached?
            NABTO_LOG_INFO(("Callback with hint: NO CRYPTO CONTEXT"));
            break;
        case UNABTO_PUSH_HINT_ENCRYPTION_FAILED:
            // uNabto core failed to encrypt the push notification
            NABTO_LOG_INFO(("Callback with hint: ENCRYPTION FAILED"));
            break;
        case UNABTO_PUSH_HINT_FAILED:
            // The Basestation received the notification but failed to handle it
            NABTO_LOG_INFO(("Callback with hint: FAILED"));
            break;
        case UNABTO_PUSH_HINT_QUOTA_EXCEEDED:
            // The basestation received the notification but retured quota exceeded, this causes the core to enforce a backoff period where no notifications will be send. This can be used to handle overloaded servers or faulty devices sending notifications at high rates.
            NABTO_LOG_INFO(("Callback with hint: QUOTA EXCEEDED"));
            break;
        case UNABTO_PUSH_HINT_QUOTA_EXCEEDED_REATTACH:
            // The basestation received the notification but return quota exceeded reattach. The uNabto core will not send notifications to the basestation before the device has been reattached. This can be used in extreme cases of faulty devices sending notifications at very high rates.
            NABTO_LOG_INFO(("Callback with hint: QUOTA EXCEEDED REATTACH"));
            break;
        default:
            NABTO_LOG_INFO(("Callback with unknown hint"));
    }
    NABTO_LOG_INFO(("Callback received context: %d",*(int*)ptr));
}

/* Initialization function called by the main function at startup
 * This can be used if the notification requires command line configuration.
 */
void demo_init() {
}

/* Function used to send webhook notifications, called from the main function */
void sendPN(void){
    int i;
    NABTO_LOG_INFO(("Sending Webhook notifications\n %s", staticData));
    push_message pm;
    
    /* Initialize the push notification structure
     * pnsid=2 lets the basestation know to use webhooks and not phone notifications
     * The static data can be any valid JSON string
     * The push_message structure also has build functions to add typical phone 
     * notification fields. See https://github.com/nabto/nabto-push-demo for usage.
     * These build functions should be reused as much as possible to avoid manipulating
     * JSON strings in c device code.
     * This example uses www.IFTTT.com as an example which only supports JSON values named
     * "value1", "value2", and "value3".
     */
    if(!init_push_message(&pm, 2, staticData)){
        NABTO_LOG_ERROR(("init_push_message failed"));
        return;
    }
    send_push_message(&pm,&callback, &context);
}

application_event_result application_event(application_request* request,
                                           unabto_query_request* query_request,
                                           unabto_query_response* query_response) {

}
