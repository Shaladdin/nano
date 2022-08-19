#include <Servo.h>

// shortcuts
#define log Serial.println

// inputs
#define metalSensor 6
#define IRSensor1 4
#define IRSensor2 5

// distanceSensor
#define echo 12
#define triger 13

const unsigned int MAX_MESSAGE_LENGTH = 12;

// global variables
const int inputs[] = {metalSensor, IRSensor1, IRSensor2};

// servo
#define servoAtas 13
#define servoBawah 12

const int servoPin[] = {servoAtas, servoBawah};
Servo servo[sizeof(servoPin) / sizeof(servoPin[0])];
#define atas [0]
#define bawah [1]

void setup()
{
    Serial.begin(115200);

    // pinmodes
    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++)
        pinMode(inputs[i], INPUT);
    for (int i = 0; i < sizeof(servoPin) / sizeof(servoPin[0]); i++)
        servo[i].attach(servoPin[i]);
}

void loop()
{
    bool trashDetected = (!digitalRead(IRSensor1)) || (!digitalRead(IRSensor2));

    // if theris trash
    if (trashDetected)
    {
        bool isMetal = !digitalRead(metalSensor);
    }
    // Check to see if anything is available in the serial receive buffer
    const char *buff = serial();
    if (buff != "")
        servo atas .write(atoi(buff));
}

const char *serial()
{
    while (Serial.available() > 0)
    {
        // Create a place to hold the incoming message
        static char message[MAX_MESSAGE_LENGTH];
        static unsigned int message_pos = 0;

        // Read the next available byte in the serial receive buffer
        char inByte = Serial.read();

        // Message coming in (check not terminating character) and guard for over message size
        if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1))
        {
            // Add the incoming byte to our message
            message[message_pos] = inByte;
            message_pos++;
        }
        // Full message received...
        else
        {
            // Add null character to string
            message[message_pos] = '\0';
            log(message);

            // Print the message (or do other things)
            // Reset for the next message
            message_pos = 0;
            return message;
        }
    }
    return "";
}