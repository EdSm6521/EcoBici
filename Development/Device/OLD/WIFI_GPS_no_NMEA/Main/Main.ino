
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <EEPROM.h>
#include <SoftwareSerial.h>

String ssid  = "GSPS0001";
String password  = "pass12345";
const char *sta_ssid  = "GSPS0001";
const char *sta_pass  = "pass12345";
/* Set these to your desired credentials. */

WiFiServer server(80);
WiFiClient client;
IPAddress  localIP;

#include "cPage.h"

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */

void standaloneServer()
{
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(sta_ssid,sta_pass);
  // WiFi.config(saip);
  localIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(localIP);
  server.begin();
  Serial.println("HTTP server started");  
}

IPAddress ssip(192,168,4,1);
IPAddress dns(255,0, 0, 0); 

static int ssConnectionTimeOut = 30;
static int ssConnectionTimeOutCount = 0;

IPAddress newIP;

bool ssConnecting = false;

static const int RXPin = 13, TXPin = 12;
static const uint32_t GPSBaud = 9600; 

SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  ss.begin(GPSBaud);
	delay(1);
	Serial.begin(9600);
	delay(1);
	EEPROM.begin(512);
	delay(1);
	standaloneServer();
}

String output = "";
String htmlOutput = "hello";
String gpgsvOutput = "";
String GPRMCOutput = "";
String GPGGAOutput = "";
int increment = 0;
bool av = false;
void loop()
{
      
      client = server.available();

      if(htmlOutput == "")
      {
        while (ss.available() > 0)
        {
          char ca = ss.read();
          //Serial.print(ca);
          //Serial.print(" - ");
          //Serial.print(ca,DEC);
          if(ca == '$')
          {
            output = "";
            av = true;
          }
          if(av)
          {
            output +=ca;
          }
          if(av == true && ca == 13)
          {
            if(output.startsWith("$GPGGA"))
            {
              GPGGAOutput = output;
            }
            if(output.startsWith("$GPRMC"))
            {
              GPRMCOutput = output;
            }
            if(output.startsWith("$GPGSV"))
            {
              gpgsvOutput = output;
            }
            if(output.startsWith("$GPGLL"))
            {
              Serial.println(output);
              htmlOutput = output;
            }
            av = false;
            output = "";
          }
        }
      }
 
      if (!client)
      {
          return;
      }

      Serial.println("new client");
      while(!client.available())
      {
        delay(1);
      }

      String req = client.readStringUntil('\r');
      Serial.println("req -> " + req);

      printPageHeader();

      htmlPrint(htmlOutput + " : " + gpgsvOutput + " : " + GPRMCOutput  + " : " + GPGGAOutput + " : " + (String)increment);

      printPageEnd();

      htmlOutput = "";

      increment++;
      
      client.flush();
      
}

