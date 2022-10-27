# crush-ping
A cute little IoT device which receives and prints messages.

Designed as a gift, this was a nice exercise in expanding my Arduino and twilio knowledge.

## What it does
The device sits idle, blinking every 15 seconds to let the user know it's working, until a message comes in (via text message). Upon receipt of a message, the button lights up solid red to let the user know something is waiting. When the button is pressed, the message is printed from the thermal printer.

# Hardware
- [Particle Photon wifi dev board](https://store.particle.io/products/photon)
- [Adafruit nano thermal printer](https://www.adafruit.com/product/2752)
- [5V, 2A switching power supply](https://www.adafruit.com/product/276)
- [Female DC power adapter](https://www.adafruit.com/product/368)
- [Metal Pushbutton with red LED ring](https://www.adafruit.com/product/559)
- [Americana Gel wood stain](https://www.amazon.com/gp/product/B09HN96TCD)
- [OLYCRAFT unfinished hexagon box 4x3"](https://www.amazon.com/gp/product/B08HLVL9T4/)
- 170pt mini breadboard and wires

# Firmware logic
## Setup
The firmware ([crushping.ino](particle/crushping.ino)) establishes a cloud function that acts as a message listener. The cloud function exposes a webhook thanks to Particle's magic, which is targeted by the twilio function ([crushping.py](twilio/crushping.py)). The device sets up a string buffer of 145 characters (bytes) which will store the most recently received message (sorry, no message queue in this implementation). It also establishes a debounce() object attached to the button's input pin, which smooths out electrical noise and unambiguously detects actual button presses. It sets a `hasMessage` boolean so we don't have to keep checking every loop if the contents of the message buffer are empty.
## Runtime
### Message handling
When a message is received, we truncate it to 145 characters and place it in our buffer, set `hasMessage` to `true`, then send a `message-received` event to the cloud to log successful receipt. 
### Main loop
If a message is waiting, we simply keep the LED on until the button is pressed. If no message is waiting, we flash the LED every 15 seconds as a heartbeat. If the button is pressed, we log a `button-pressed` event to the cloud, but nothing else happens if no message is in the buffer.
### Message printing
If a message is in the buffer when the button is pressed, we print the message (including XOXO lines above and below and some extra space to make the printout easy to tear off) and register a `message-printed` event to the cloud. We then clear the message buffer and set `hasMessage` back to `false`.
## Dependencies
- [Adafruit thermal printer library](https://github.com/adafruit/Adafruit-Thermal-Printer-Library)
- [Particle debounce library](https://docs.particle.io/reference/device-os/libraries/d/Debounce/)

# References and inspirations
https://docs.particle.io/getting-started/integrations/webhooks/

https://docs.particle.io/reference/device-os/api/cloud-functions/particle-publish/

https://www.hackster.io/gatoninja236/2-way-particle-photon-communication-011f02

https://docs.particle.io/reference/device-os/api/string-class/reserve/

https://github.com/thomasfredericks/Bounce2

https://particle.hackster.io/boldbigflank/love-notes-37be49 (very helpful as a starting place for code logic and twilio integration)

https://particle.hackster.io/middleca/sending-sound-over-the-internet-f097b4
