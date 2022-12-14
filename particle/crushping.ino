// This #include statement was automatically added by the Particle IDE.
#include "Debounce.h"

// Prereq for thermal library?
#include <Particle.h>
#include "Adafruit_Thermal.h"

// Define static variables
#define LOW_DURATION 15000 // milliseconds
#define HIGH_DURATION 250 // milliseconds
#define PRINTER_RATE 9600 // hertz/baud, Adafruit nano printer

// Declare global variables
Adafruit_Thermal printer;

String lastMessage;
bool hasMessage;

unsigned long duration;
unsigned long startTime;
unsigned long currentTime;

Debounce debouncer = Debounce();

int LED_PIN = D7;
int BUTTON_PIN = D0;

//-------------------------------------------------------------

// Clear the internal message buffer
void clearMessage() {
    lastMessage = "";
    hasMessage = false;
}

void setMessage(String msg) {
    // Write message (truncated for buffer size) to internal buffer
    lastMessage = msg.substring(0,144);
    hasMessage = true;
}

// This function will print whatever message is sent to it
int receiveMessage(String msg) {
    // debug: send an event saying we received a message
    Particle.publish("message-received", msg);
    setMessage(msg);
    // When the function is done, return 1 so the sender knows it worked
    return 1;
}

//Print a message 
void printMessage(String msg) {
    // Apparently, we must wake the printer from low power mode before interacting with it
    printer.wake();
    printer.setDefault();
    // Print the message with some ascii borders (32 characters wide)
    printer.println("XOXOXOXOXOXOXOXOXOXOXOXOXOXOXOX");
    printer.println(msg);
    printer.println("XOXOXOXOXOXOXOXOXOXOXOXOXOXOXOX");
    // Done printing, feed it some paper to move above the tear line
    printer.feed(2);
    // Put printer in low power mode between prints (this happens automatically after certain time)
    printer.sleep();
    //delay(1000L);
    // debug: send an event saying we printed a message
    Particle.publish("message-printed", msg);
}

// If we have a message in our buffer, print it and then clear the buffer
void printIfMessage() {
    if (hasMessage) {
        printMessage(lastMessage);
        clearMessage();
    }
}

void setup() {
    // Expose Particle cloud variables and functions
    Particle.function("message", receiveMessage);
    Particle.variable("hasMessage", hasMessage);
    Particle.variable("lastMessage", lastMessage);

    // Serial1 is configured to the RX and TX pins on the Photon
    Serial1.begin(PRINTER_RATE);
    // Start communicating with the printer via the Serial1 pins
    printer.begin(&Serial1);
    
    // Preallocate up to 145 characters (bytes) for lastMessage
    lastMessage.reserve(145);
    lastMessage = "empty";

    //Set up [button] LED
    pinMode(LED_PIN, OUTPUT); // set LED pin to output
    duration = LOW_DURATION;
    startTime = millis();

    //Set up button
    debouncer.attach(BUTTON_PIN, INPUT_PULLUP);
    debouncer.interval(80); // minimum push interval in ms
}

void loop() {
    currentTime = millis();
    // check button status
    debouncer.update();
    // If we have a message, just keep the LED on
    if (hasMessage) {
        digitalWrite(LED_PIN, HIGH);
    // If we don't have a message, flash the LED every [5] seconds so we know we're alive
    } else {
        if (currentTime - startTime > duration) {
            if (duration == LOW_DURATION) {
                digitalWrite(LED_PIN, HIGH);
                duration = HIGH_DURATION;  // next period is HIGH, so set correct duration
            } else {
                digitalWrite(LED_PIN, LOW);
                duration = LOW_DURATION;
            }
            startTime = currentTime;  // pin has just changed state, so start timing again
        }
    }
    // if button is pressed, print the currently buffered message if we have one
    if (debouncer.rose()) {
        Particle.publish("button-pressed", "true"); // debug: send an event saying button was pressed
        printIfMessage();
    }
}