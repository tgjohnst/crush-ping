// This #include statement was automatically added by the Particle IDE.
#include <Debounce.h>

// Prereq for thermal library?
#include <Particle.h>

// This #include statement was automatically added by the Particle IDE.
//#include <Adafruit_Thermal.h>

// Declare global variables
//Adafruit_Thermal printer; // baud rate 9600 for the nano printer
String lastMessage;
Debounce debouncer = Debounce();


void setup() {
pinMode(D7, OUTPUT); // set LED pin to output
debouncer.attach(D0, INPUT_PULLUP);
debouncer.interval(80); // interval in ms
}

void loop() {
// check button status
debouncer.update();
// if button is pressed, light LED
if (debouncer.read() == HIGH) {
digitalWrite(D7, HIGH);
} else {
digitalWrite(D7, LOW);
}
}