// This #include statement was automatically added by the Particle IDE.
#include "Debounce.h"

// Prereq for thermal library?
#include <Particle.h>
#include "Adafruit_Thermal.h"

// This #include statement was automatically added by the Particle IDE.
//#include <Adafruit_Thermal.h>

// Declare global variables
Adafruit_Thermal printer; // nano printer (9600 baud)
String lastMessage;
bool hasMessage;
Debounce debouncer = Debounce();

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
    // Print the message with some ascii borders (32 characters wide)
    printer.println("XOXOXOXOXOXOXOXOXOXOXOXOXOXOXOX");
    printer.println(msg);
    printer.println("XOXOXOXOXOXOXOXOXOXOXOXOXOXOXOX");
    // Done printing, feed it some paper to move above the tear line
    printer.feed(3);
    // Reset
    printer.setDefault();
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
    // Set up the Particle variable and function, then connect
    // to the Particle Cloud
    Particle.function("message", receiveMessage);
    Particle.variable("hasMessage", hasMessage);

    // Serial1 is configured to the RX and TX pins on the Photon
    Serial1.begin(9600);
    // Start communicating with the printer via the Serial1 pins
    printer.begin(&Serial1);
    
    // Preallocate up to 145 characters (bytes) for lastMessage
    lastMessage.reserve(145);
    lastMessage = "empty" ;

    //Set up LEDs for button
    pinMode(D7, OUTPUT); // set LED pin to output
    debouncer.attach(D0, INPUT_PULLUP);
    debouncer.interval(80); // interval in ms
}

void loop() {
    // check button status
    debouncer.update();

    if (hasMessage) {
        digitalWrite(D7, HIGH);
    } else {
        digitalWrite(D7, LOW);
    }
    // if button is pressed, print the currently buffered message if we have one
    if (debouncer.rose()) { // if button is pressed
        // debug: send an event saying button was pressed
        Particle.publish("button-pressed", "true");
        printIfMessage();
    }
}