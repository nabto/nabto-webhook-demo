# Nabto Webhook Demo

To run this demo, you need a Nabto device ID and key. This device ID must be on a domain where you have access to configure webhooks.

## Usage
Clone this repository with the recursive argument to also get the unabto submodule:
```
git clone --recursive https://github.com/nabto/nabto-webhook-demo.git
```

go to the repository folder, and build the uNabto device:
```
cd nabto-webhook-demo
mkdir build
cd build
cmake ..
make -j
```

This example uses https://webhook.site as endpoint for the webhook as this is easily available to anyone. However, if you have another webhook endpoint available, this can be used instead, and the next section can be skipped.

### Configuring webhook.site
Go to https://webhook.site. The webhook site should immediately redirect you to your own unique URL you can use for testing the Nabto push webhooks. Note down your unique URL, this is this URL which should be registered in the Nabto console, and is also the website where received push notifications can be seen.

### Running Nabto

Go to https://console.cloud.nabto.com/ and configure your domain to use your webhook URL. If you are using your own endpoint, a username and password for HTTP basic access authentication can be configured as well. The webhook.site example does not use any authentication.

Then run the uNabto device build earlier by changing <DEVICE ID> and <DEVICE KEY> to your device ID and pre shared key found in the Nabto console and run the following command:
```
./webhook_demo_device -d <DEVICE ID> -k <DEVICE KEY>
```
Once the Nabto webhook demo has started, a log line should appear stating that the device has attached to the Nabto basestation:
```
08:53:04:539 unabto_attach.c(591) State change from WAIT_GSP to ATTACHED
```
Now, press the enter key to trigger a notification. After a few seconds, the notification should arrive at your configured webhook. If you are using the webhook.site example, the notification should automatically appear on the website. 

## Implementation

This demo is implemented in the two files `src/unabto_main.c` and `src/unabto_application.c`

`unabto_main.c` initializes the uNabto software, after witch it runs an infinite loop which checks for keyboard input to trigger notifications, and lets the uNabto software run every 10ms.

`unabto_application.c` implements the handling of notifications. Here is a function to construct and send a notification, as well as a callback function which will be called when the notification is successfully delivered to the Nabto basestation.
