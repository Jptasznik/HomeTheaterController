#include <Arduino.h>

/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 * This must be done before the #include <IRremote.hpp>
 */
#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC

#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
//#define DECODE_LG
//#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER

//#define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
//#define DECODE_HASH         // special decoder for all protocols

//#define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!

//#define DEBUG               // Activate this for lots of lovely debug output from the decoders.

//#define RAW_BUFFER_LENGTH  180  // Default is 112 if DECODE_MAGIQUEST is enabled, otherwise 100.
//#include "RemotePinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>

typedef enum {
  Unknown_Remote_Code = 0,
  No_IR_Received,
  Denon_X3700H_Power_On,
  Denon_X3700H_Power_Off,
  Denon_Channel_2,
  Denon_Channel_3
} remote_code_type_t;


/////////////////////////////////////////////////
///
///   Nest
///
/////////////////////////////////////////////////
class RemoteControl 
{
  public:
    void begin(int irReceivePin, bool enableLEDFeedback){
      // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
      IrReceiver.begin(irReceivePin, enableLEDFeedback);
      
  //    Serial.print(F("Ready to receive IR signals of protocols: "));
 //     printActiveIRProtocols(&Serial);
 //     Serial.println("at pin " + irReceivePin); 
    }

    remote_code_type_t GetRemoteCode() {
        /*
        * Check if received data is available and if yes, try to decode it.
        * Decoded result is in the IrReceiver.decodedIRData structure.
        *
        * E.g. command is in IrReceiver.decodedIRData.command
        * address is in command is in IrReceiver.decodedIRData.address
        * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
        */
      if (!IrReceiver.decode()) {
        return No_IR_Received;
      }
      /*
        * !!!Important!!! Enable receiving of the next value,
        * since receiving has stopped after the end of the current received data packet.
        */
      IrReceiver.resume(); // Enable receiving of the next value

      /*
        * Print a short summary of received data
        */
   //   IrReceiver.printIRResultShort(&Serial);
     // IrReceiver.printIRSendUsage(&Serial);
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
       //   Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
          // We have an unknown protocol here, print more info
         // IrReceiver.printIRResultRawFormatted(&Serial, true);
          return Unknown_Remote_Code;
      }
      // Serial.println();

      /*
        * Finally, check the received data and perform actions according to the received command
        */
      // DENON CHANNEL 2
      if (IrReceiver.decodedIRData.protocol == DENON && IrReceiver.decodedIRData.address == 0x1 && IrReceiver.decodedIRData.command == 0x2) 
      {
        return Denon_Channel_2;
      }
      // DENON CHANNEL 3
      if (IrReceiver.decodedIRData.protocol == DENON && IrReceiver.decodedIRData.address == 0x1 && IrReceiver.decodedIRData.command == 0x3) 
      {
        return Denon_Channel_3;
      }
      // DENON X3700H POWER ON
      if (IrReceiver.decodedIRData.protocol == KASEIKYO_DENON && IrReceiver.decodedIRData.address == 0x614 && IrReceiver.decodedIRData.command == 0x0) 
      {
        return Denon_X3700H_Power_On;
      }
      // DENON X3700H POWER OFF
      if (IrReceiver.decodedIRData.protocol == KASEIKYO_DENON && IrReceiver.decodedIRData.address == 0x714 && IrReceiver.decodedIRData.command == 0x0) 
      {
        return Denon_X3700H_Power_Off;
      }        

      return Unknown_Remote_Code;
    }
};