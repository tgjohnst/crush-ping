// This was originally sourced from Alex Swan's lovenotes project at https://particle.hackster.io/boldbigflank/love-notes-37be49

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Thermal.h>
#include "cellular_hal.h"

Adafruit_Thermal printer;


// Connect to the Particle Cloud when we say so, so that we can set up
// cloud variables and functions and only connect once
SYSTEM_MODE(SEMI_AUTOMATIC)

// Soracom.io Credentials
STARTUP(cellular_credentials_set("soracom.io", "sora", "sora", NULL));


// This variable is for double checking the device is communicating
bool ready = false;

// This function will print whatever message is sent to it
int receiveMessage(String msg) {
    // For debug purposes, send an event saying we received a message
    Particle.publish("message-received", msg, PUBLIC);
    // Print the message with some ascii borders (32 characters wide)
    printer.println("XOXOXOXOXOXOXOXOXOXOXOXOXOXOXOXO");
    printer.println(msg);
    printer.println("XOXOXOXOXOXOXOXOXOXOXOXOXOXOXOXO");
    // Done printing, feed it some paper to move above the tear line
    printer.feed(2);
    // Reset
    printer.setDefault();
    // When the function is done, return 1 so the sender knows it worked
    return 1;
}

void setup() {
    // Set up the Particle variable and function, then connect
    // to the Particle Cloud
    Particle.variable("ready", ready);
    Particle.function("message", receiveMessage);
    Particle.connect();
    
    // Serial1 is configured to the RX and TX pins on the Electron
    Serial1.begin(19200);
    // Start communicating with the printer via the Serial1 pins
    printer.begin(&Serial1);
    
    ready = true;
}

void loop() {
    // Unused in this implementation
}