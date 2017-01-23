
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <EEPROM.h>
#include <SoftwareSerial.h>

String ssid  = "GPS0001";
String password  = "pass12345";
const char *sta_ssid  = "GPS0001";
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
  Serial.print("SSID :" );
  Serial.println(ssid);
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

String htmlOutput = "";
String htmlOutputLast = "";
int increment = 0;

void loop()
{
      
      client = server.available();

      if(increment < 10)
      {
        while (ss.available() > 0)
        {
          char ca = ss.read();
           Serial.print(ca);
          //Serial.print(" - ");
          //Serial.print(ca,DEC);
           htmlOutput += ca;
           if(ca == 10 || ca == 13)
           {
             increment++;
           }
           if(increment >= 10)
           {
             htmlOutputLast += ca;
           }
        }
      }

      if (!client)
      {
          delay(1);
          return;
      }

      Serial.println("new client");
      while(!client.available())
      {
        delay(1);
      }

      String req = client.readStringUntil('\r');
      Serial.println("req -> " + req);
      client.flush();
      
      if (req.indexOf("/get_gps_data") != -1)
      {
          Serial.print(htmlOutput);
          client.print(htmlOutput);
      }

      htmlOutput = htmlOutputLast;
      increment = 0;
      
}

