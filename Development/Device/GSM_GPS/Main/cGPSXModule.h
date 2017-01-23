
#ifndef GPS_MODULE_H
#define GPS_MODULE_H  

#include <SoftwareSerial.h>

class cGPSXModule
{

  cTimer updateRate;
  SoftwareSerial *ss;
  
public:
	bool initialized = false;
	uint32_t GPSBaud = 9600;
	String htmlOutput;

  cGPSXModule()
  {
	  
  }

	void setup(SoftwareSerial *_ss,String _deviceName,int _GPSBaud = 9600,int _delay = 10)
	{
		ss = _ss;
		GPSBaud = _GPSBaud;
		ss->begin(GPSBaud);
		ciD.println("GPS Online");
		delay(100);
		initialized = true;
		htmlOutput = "";
	}
 
	String Result = "";
	const int MAX_DATA_COUNT = 4;
	int DATA_COUNT = 0;

	bool validChar(char _ca)
	{
		if (_ca >= 65 && _ca < 91) { return true; }
		if (_ca == ',') { return true; }
		if (_ca == '.') { return true; }
		if (_ca == '$') { return true; }
		if (_ca == '*') { return true; }
		if (_ca >= 0 && _ca <= 57) { return true; }
		return false;
	}

	String loop()
	{
		if (!initialized) { return ""; }
		if (!updateRate.loop()) { return ""; }
		String line = "";
		// ciD.println("GPS Loop");
		while (ss->available() > 0)
		{
			char ca = ss->read();

			if (ca == 10 || ca == 13 || ca == 255 || ca == -1 || ca == 0 || !validChar(ca))
			{

			}
			else
			{
				if (htmlOutput.length() == 0)
				{
					if (ca != '$') { continue; }
				}
				htmlOutput += ca;
				// ciD.println((String)htmlOutput.length());
				if (htmlOutput.length() > 100)
				{
					// ciD.print(htmlOutput);
					return htmlOutput; 
				}
			}

		
		}
		return htmlOutput;
	}

 String getData()
 {
  // if (!initialized) { return ""; }
  return htmlOutput;
 }

};

#endif

