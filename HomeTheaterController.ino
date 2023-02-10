#include <Arduino.h>
#include "RemoteControl.h"
#include "Lutron.h"
#include "IOTRelay.h"

#define WIFI_SSID           "YOUR_SSID"
#define WIFI_PASSWORD       "YOUR_WIFI_PASSWORD" 
#define RELAY_PIN           12 //D6 on ESP8266
#define IR_RECEIVE_PIN      14 //D5 on ESP8266
#define LUTRON_HOST         "YOUR_LUTRON_BRIDGE_PRO_IP"
#define LUTRON_PORT         23
#define LUTRON_USER         "YOUR_LUTRON_USER"
#define LUTRON_PASSWORD     "YOUR_LUTRON_PASSWORD"

LutronControl     lutron;
IOTRelayControl   relay;
RemoteControl     remote;

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println(""); 
    // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing Remote library version " VERSION_IRREMOTE));


  relay.begin(RELAY_PIN);
  remote.begin(IR_RECEIVE_PIN, false);
  lutron.begin(LUTRON_HOST, LUTRON_PORT, LUTRON_USER, LUTRON_PASSWORD);

  Serial.println("Start Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Wifi Started");    
  Serial.println("done with setup");
}

void CheckForIR() {
  remote_code_type_t remteCode = remote.GetRemoteCode();
  switch (remteCode) {
    case Denon_X3700H_Power_On:
      Serial.println("DENON X3700H Power ON Button Pressed");              
      relay.TurnOn();
      break;
    case Denon_X3700H_Power_Off:
      Serial.println("DENON X3700H Power OFF Button Pressed");              
      relay.TurnOff();
      break;
    case Denon_Channel_2:
      Serial.println("DENON 2 Button Pressed - Turn on Basement Lights");              
      lutron.BasementLights(100);
      break;
    case Denon_Channel_3:
      Serial.println("DENON 3 Button Pressed - Turn off Basement Lights");              
      lutron.BasementLights(0);
      break;
  }
}

bool lutronConnected = false;
bool wiFiConnected = false;
long int lastConnected = millis();
long int RebootTimeMs = 1000 * 60 * 10; // 10 Minutes 

bool WiFiConnected() {
  if (WiFi.status() != WL_CONNECTED)
  {
    if (wiFiConnected) {
      wiFiConnected = false;
      Serial.print("WiFi Disconnected!");      
    }
    // If the Relay is off and it has been too long since connected to WiFi reboot
    if (!relay.IsOn() && (millis() - lastConnected) > RebootTimeMs) {
      Serial.print("WiFi Disconnected for too long...rebooting.");      
      ESP.restart();      
    }
  } else {
    lastConnected = millis();
    if (!wiFiConnected) {
      wiFiConnected = true;
      Serial.print("WiFi Connected! IP address: ");
      Serial.println(WiFi.localIP());
    }
  }
  return wiFiConnected;
}

void loop() {
  CheckForIR();
  if (WiFiConnected() && lutron.VerifyLutronConnection()) {
    if (!lutronConnected) {
      lutronConnected = true;
      Serial.println("Lutron Connected!");
    }
  } else {
    if (lutronConnected) {
      lutronConnected = false;
      Serial.println("Lutron Disconnected!");
    }
  }
}