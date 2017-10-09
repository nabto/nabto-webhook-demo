# Nabto Webhook Demo

To run this demo, you need a Nabto device ID and key. This device ID must be on a domain where webhooks are configured.

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

This example uses www.ifttt.com as endpoint for the webhook as this is easily available to anyone. However, if you have another webhook endpoint available, this can be used instead, and the next section can be skipped.

### Configuring ifttt
Go to www.ifttt.com and create an account, or login with google or facebook and follow the following steps:

 1. Go to "My Applets"
 2. Click "New Applet"
 3. Click "this"
 4. search for webhooks and click the webhooks service
 5. Click the box
 6. Choose an event name using characters usable in an URL (eg. temperature_notification)
 7. Click "Create trigger"
 8. Click "that"
 9. Choose a service to send the notification to (eg. email)
 10. Configure the email you want to send when a notification occurs
 11. Go to the webhooks service and click "Documentation"
 12. Note down the URL for triggering an event, change "{event}" to the event name chosen in step 6

### Running Nabto

Go to https://api.cloud.nabto.com/dashboard/ and configure your domain to use your webhook URL. If you are using the ifttt example above, this is the URL from step 12. Otherwise, configure your own webhook endpoint.

Then run the uNabto device build earlier by changing <DEVICE ID> and <DEVICE KEY> to your device ID and pre shared key and running the following command:
```
./webhook_demo_device -d <DEVICE ID> -k <DEVICE KEY>
```

