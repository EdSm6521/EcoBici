
// #include <AltSoftSerial.h>
#include "cD.h"
#include "cGSM.h"
#include "cTimer.h"
#include "cGPSXModule.h"
#include <SoftwareSerial.h>

// AltSoftSerial altSerial;
SoftwareSerial ss(PIND2,PIND3); // RX, TX

static cTimer		updateInterval;
static cGPSXModule	GPSModule;
static cGSM			GSModule;

int a6PowerPin = 11;

void setup()
{
	pinMode(a6PowerPin,OUTPUT);
	ciD.setup();
	ciD.println(" Starting ... ");
	updateInterval.setup(5000);
	GPSModule.setup(&ss, "GPS");
}

void reset()
{
  digitalWrite(a6PowerPin,HIGH);
  delay(2000);
  digitalWrite(a6PowerPin,LOW);
}

void loop()
{
 
  String xyresult = GPSModule.loop();

  if (xyresult.length() >= 60)
  {
        if (xyresult == "")
        {
          xyresult = "NOGPSDATA";
        }
        ss.flush();
        ciD.println("GPS data : " + xyresult);
        delay(1000);
        ss.listen();
        reset();
        GSModule.setup();
        if (GSModule.connect())
        {
          //altSerial.println("Conexion exitosa");
        }
        if (GSModule.send("[EB1][BAD]" + xyresult))
        {
         // altSerial.println("Enviada data");
        }
        delay(20000);
        GPSModule.setup(&ss, "GPS");
        xyresult = "";
        GPSModule.htmlOutput = "";
  }

 delay(1);
 
}

