# HomeTheaterController
I created this so that I can turn on and off my AMP which does not have discrete power on and off IR codes.
This device listens for my Denon X3700H Power on and off IR commands and Turns on and off the outlet that my AMP is conneced on the IOT Relay.
It also listens for Channel 2 and 3 IR commands not used in my setup to turn Lutron Lights on and off.

# Hardware
- ESP8266 NodeMCU CP2102 ESP-12E: https://a.co/5ANd7af
- Gikfun Digital 38khz Ir Receiver Ir Transmitter Sensor Module Kit for Arduino:  https://a.co/3uIlNy8
<B>NOTE:  I Only use the receiver from this kit in this project</B>
- Lutron Caseta L-BDGPRO2-WH - SmartBridge Pro: https://a.co/2Rgr5F5
<B>NOTE: The non Pro version of the SmartBridge does NOT support the Telnet control</b>
- IOT Relay for turning on and off AC outlets: https://www.digital-loggers.com/iot2.html
