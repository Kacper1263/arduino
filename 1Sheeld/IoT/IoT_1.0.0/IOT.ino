

#define CUSTOM_SETTINGS
#define INCLUDE_IOT_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_EMAIL_SHIELD
#define INCLUDE_CAMERA_SHIELD

/* Set your host name */
#define HOST_NAME "your-brocker-here.com"

/* Include 1Sheeld library. */
#include <OneSheeld.h>
#include <Servo.h>
Servo myservo;
int lastPos = 90;
int pos = 90;
bool flash = false;

/* Set an LED on pin 13. */
int ledPin = 13;
/* Subscribe to topic 1Sheeld/MyArduino/led . */
const char * myTopic = "1sheeld/km/led";
const char * myTopicServo = "1sheeld/km/servo";

void setup()
{
    /* Start communication. */
    OneSheeld.begin();

    myservo.attach(9);

    /* Disconnect from broker. */
    IOT.disconnect();
    /* Reset all connection variables to default */
    IOT.resetConnectionParametersToDefaults();
    /* Connect to mosquitto's public broker. */
    IOT.connect(HOST_NAME);
    /* Subscribe to new messages. */
    IOT.setOnNewMessage(&newMessage);
    /* Subscribe to connnection status callback. */
    IOT.setOnConnectionStatusChange(&connectionStatus);
    /* Subscribe to error callback. */
    IOT.setOnError(&error);
    /* Some time for app to connect. */
    delay(3000);
    /* Subscribe to led topic. */
    IOT.subscribe(myTopic);
    IOT.subscribe(myTopicServo);
    /* LED pin mode output. */
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    IOT.publish("1sheeld/km/data", "Arduino ON", QOS_0);
}

void loop()
{}

void newMessage(char * incomingTopic, char * payload, byte qos, bool retained)
{
    /* Check on incomingTopic. */
    if (!strcmp(myTopic, incomingTopic))
    {
        /* If payload states ON. */
        if (!strcmp("ON", payload))
        {
            flash = true;
            /* Turn off the warning led. */
            digitalWrite(ledPin, LOW);
            IOT.publish("1sheeld/km/data", "Flash ON", QOS_0);
        }
        /* If payload states OFF. */
        else if (!strcmp("OFF", payload))
        {
            flash = false;
            /* Turn on the warning led. */
            digitalWrite(ledPin, HIGH);
            IOT.publish("1sheeld/km/data", "Flash OFF", QOS_0);
        }
        else if (!strcmp("IMG", payload))
        {
            if (flash == true) {
                Camera.setFlash(ON);
            }
            else {
                Camera.setFlash(OFF);
            }
            /* Take the picture. */
            Camera.rearCapture();
            /* Wait for 10 seconds. */
            OneSheeld.delay(10000);

            Email.attachLastPicture("marcinkiewicz.kacper@gmail.com", "IoT", "img from IoT app");

            IOT.publish("1sheeld/km/data", "Photo has been sent", QOS_0);
        }

        /* If payload states ON. */
        if (!strcmp("100", payload))
        {
            /* Turn on the led. */
            digitalWrite(ledPin, HIGH);
        }
        /* If payload states OFF. */
        else if (!strcmp("0", payload))
        {
            /* Turn off the led. */
            digitalWrite(ledPin, LOW);
        }    
    }

    if (!strcmp(myTopicServo, incomingTopic)) {
        pos = atoi(payload);
        pos = map(pos, 0, 180, 180, 0);
        if (lastPos > pos) {                    // ">" because now 0 is 180
            while (lastPos > pos) {
                lastPos--;
                myservo.write(lastPos);
                delay(50);
                
            }
            IOT.publish("1sheeld/km/data", "Position set", QOS_0);
        }
        else if (lastPos < pos) {                    // "<" because now 0 is 180
            while (lastPos < pos) {
                lastPos++;
                myservo.write(lastPos);
                delay(50);

            }
            IOT.publish("1sheeld/km/data", "Position set", QOS_0);
        }
    }
}

void connectionStatus(byte statusCode)
{
    /* Check connection code and display. */
    switch (statusCode)
    {
    case CONNECTION_SUCCESSFUL: Terminal.println("CONNECTION_SUCCESSFUL"); break;
    case CONNECTION_FAILED: Terminal.println("CONNECTION_FAILED"); break;
    case CONNECTION_LOST: Terminal.println("CONNECTION_LOST"); break;
    case CONNECTION_LOST_RECONNECTING: Terminal.println("CONNECTION_LOST_RECONNECTING"); break;
    case NOT_CONNECTED_YET:  Terminal.println("NOT_CONNECTED_YET"); break;
    case MISSING_HOST: Terminal.println("MISSING_HOST"); break;
    }
}

void error(byte errorCode)
{
    /* Check error code and display. */
    switch (errorCode)
    {
    case CONNECTION_REFUSED: Terminal.println("CONNECTION_REFUSED"); break;
    case ILLEGAL_MESSAGE_RECEIVED: Terminal.println("ILLEGAL_MESSAGE_RECEIVED"); break;
    case DROPPING_OUT_GOING_MESSAGE: Terminal.println("DROPPING_OUT_GOING_MESSAGE"); break;
    case ENCODER_NOT_READY: Terminal.println("ENCODER_NOT_READY"); break;
    case INVALID_CONNACK_RECEIVED: Terminal.println("INVALID_CONNACK_RECEIVED"); break;
    case NO_CONNACK_RECEIVED: Terminal.println("NO_CONNACK_RECEIVED"); break;
    case CONNACK_UNACCEPTABLEP_ROTOCOLVERSION: Terminal.println("CONNACK_UNACCEPTABLEP_ROTOCOLVERSION"); break;
    case CONNACK_IDENTIFIER_REJECTED: Terminal.println("CONNACK_IDENTIFIER_REJECTED"); break;
    case CONNACK_SERVER_UNAVAILABLE: Terminal.println("CONNACK_SERVER_UNAVAILABLE"); break;
    case CONNACK_AUTHENTICATION_FAILED: Terminal.println("CONNACK_AUTHENTICATION_FAILED"); break;
    case CONNACK_NOT_AUTHORIZED: Terminal.println("CONNACK_NOT_AUTHORIZED"); break;
    case CONNACK_RESERVED: Terminal.println("CONNACK_RESERVED"); break;
    }
}