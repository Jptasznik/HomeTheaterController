#include <Arduino.h>
#include <ESP8266WiFi.h>

// Light IDs
typedef enum {
  KitchenLights = 2,
  DiningRoomLights = 4,
  LaundryRoomLights = 6,
  StairsLights = 8,
  BasementGymLights = 10,
  BasementPlayroomLights = 11,
  MasterBedroomFan = 14
} lutron_type_t;

/////////////////////////////////////////////////
///
///   Nest
///
/////////////////////////////////////////////////
class LutronControl 
{
  private:
    WiFiClient luttronClient; //For ESP8266 boards
    const char* lutronHost = "";
    const char* lutronUser = "";
    const char* lutronPassword = "";
    int lutronPort = 23;
    bool lutronLoggedIn = false;

    /*******************************************/
    //   Send Lutron Command - turn on and off devices
    /*******************************************/
    void SendLutronCommand(char *command)
    {
      luttronClient.println(command);
      //Serial.println(command);
      String line = luttronClient.readStringUntil('\n');
   /*   if (line.length() > 0 )
      {
        Serial.println(line);
      }
   */
    }

  public:
    void begin(const char* lutronHost, int lutronPort, const char* lutronUser, const char* lutronPassword)   {
      this->lutronHost = lutronHost;
      this->lutronPort = lutronPort;
      this->lutronUser = lutronUser;
      this->lutronPassword = lutronPassword;
    }
    /*******************************************/
    //   Verify and Log into the Lutron Server
    /*******************************************/
    bool VerifyLutronConnection()
    {
      // Check if disconnected
      if (!luttronClient.connected())
      {
          lutronLoggedIn = false;
          if (!luttronClient.connect(lutronHost, lutronPort)) 
          {
            Serial.println("Failed to connect to Lutron smart hub pro");
            return false;
        }      
        Serial.println("Connected to Lutron smart hub pro");
        luttronClient.setNoDelay(true);
        luttronClient.setTimeout(100);
      }

      // Check if logged in
      if (!lutronLoggedIn)
      {
          String line = "";
          while (luttronClient.connected()) 
          {
            String line = luttronClient.readStringUntil('\n');
            if (line == "login: ") 
            {
              luttronClient.print(this->lutronUser);
              luttronClient.print("\r\n");
            }
            else if (line == "password: ") 
            {
              luttronClient.print(this->lutronPassword);
              luttronClient.print("\r\n");
            }
            else if (line != "GNET> ")
            {
              Serial.println("logged into lutron smart hub pro");
              lutronLoggedIn = true;
              break;
            }  
          }  
      }

      return lutronLoggedIn;
    }

    void BasementLights(int level) {
      SetLightLevel(BasementGymLights, level);
      SetLightLevel(BasementPlayroomLights, level);
    }
    
    void SetLightLevel(lutron_type_t lightId, int level) {
      if (!lutronLoggedIn){
        return;        
      }
      char command[32];
      snprintf(command, sizeof(command), "#OUTPUT,%i,1,%i", lightId, level);
      SendLutronCommand((char*)&command);
    };

};