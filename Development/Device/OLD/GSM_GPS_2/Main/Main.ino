

#include <AltSoftSerial.h>
#include "cD.h"
#include "cGSM.h"
#include "cTimer.h"
#include "Sensors/cGPSModule.h"
#include <SoftwareSerial.h>

#include "cEEPROM.h"

AltSoftSerial altSerial;
SoftwareSerial ss(PIND2,PIND3); // RX, TX

static cTimer		updateInterval;
static cGPSModule	GPSModule;
static cGSM			GSModule;

int a6PowerPin = 11; 

int EEPROM_STATE_ID = -1;
int EEPROM_VALUE_ID = -1;

String EEPROM_STATE_STR = "";
String EEPROM_VALUE_STR = "";

void reset()
{
  digitalWrite(a6PowerPin,HIGH);
  delay(2000);
  digitalWrite(a6PowerPin,LOW);
}

bool lonce = 0;

void setup()
{
  ciD.setup();
  ciD.enable = true;
  ciD.println("Start ...");
  pinMode(a6PowerPin,OUTPUT);
  delay(1000);
  reset();

	eeprom.setup(512);

	EEPROM_STATE_ID = eeprom.addData(1);
	EEPROM_VALUE_ID = eeprom.addData(200);

	EEPROM_STATE_STR = eeprom.read(EEPROM_STATE_ID);
	EEPROM_VALUE_STR = eeprom.read(EEPROM_VALUE_ID);

	ciD.println(" EEPROM_STATE_STR " + EEPROM_STATE_STR);
	ciD.println(" EEPROM_VALUE_STR " + EEPROM_VALUE_STR);
	
	if (EEPROM_STATE_STR != "1" || EEPROM_STATE_STR != "0")
	{
		eeprom.write(EEPROM_STATE_ID, "0");
		EEPROM_STATE_STR = "0";
	}
 
	if (EEPROM_STATE_STR == "0")
	{
		GPSModule.setup(&ss, "GPS");
    lonce = 1;
	}
	if (EEPROM_STATE_STR == "1")
	{
		GSModule.setup(&altSerial);
		if (GSModule.connect())
		{
			altSerial.println("Conexion exitosa");
		}
		if (GSModule.send("[BAD][ECOBICI]" + EEPROM_VALUE_STR))
		{
			altSerial.println("Enviada data");
		}
		eeprom.write(EEPROM_STATE_ID, "0");
		reset();
	}
	
}



void loop()
{
	
  // if (EEPROM_STATE_STR == "1") { return; }
  // if(!lonce){return;}
  String xyresult = GPSModule.loop();
  if (xyresult.length() >= 150)
  {
        ciD.println("GPS get data");
		    String sa = xyresult;
        ciD.println("GPS get data result");
				if (sa == "")
				{
					sa = "NOGPSDATA";
				}
				ciD.println("GPS data : " + sa);
				// eeprom.write(EEPROM_VALUE_ID, sa);
				lonce = 0;
				reset();
			 //
	}
	
	GSModule.setup(&altSerial);
	if (GSModule.connect())
	{
		altSerial.println("Conexion exitosa");
	}
	if (GSModule.send("[ECOBICI][BAD][GPSDATA,]"))
	{
		altSerial.println("Enviada data");
	}

	delay(20000);

}

