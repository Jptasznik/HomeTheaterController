#include <Arduino.h>

/////////////////////////////////////////////////
///
///   Nest
///
/////////////////////////////////////////////////
class IOTRelayControl 
{
  private:
    int RelayPin = 12;
    bool isOn = false;

  public:
    bool IsOn() {
      return this->isOn;
    }

    void begin(int relayPin){
      this->RelayPin = relayPin;
      pinMode(this->RelayPin, OUTPUT);
      digitalWrite(this->RelayPin, LOW);
    }

    void TurnOn() {
        digitalWrite(this->RelayPin, HIGH);
    }

    void TurnOff() {
        digitalWrite(this->RelayPin, LOW);
    }
};