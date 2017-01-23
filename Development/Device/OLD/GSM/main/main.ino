#include <SoftwareSerial.h>
#include "cGSM.h"
#include <AltSoftSerial.h>

AltSoftSerial portTwo;
cGSM gsm;

void setup()
{
  portTwo.begin(9600);
  
  gsm.setup(&portTwo, 0);
  if(gsm.connect())
    portTwo.println("Conexion exitosa");
  if(gsm.send("HERNAN HABLANDO A RUSO BAD"))
    portTwo.println("Enviada data");
   
}
void loop() {
  if(Serial.available()){
    portTwo.write(Serial.read());
  }
  if (portTwo.available()){
    Serial.write(portTwo.read());
  }
}






