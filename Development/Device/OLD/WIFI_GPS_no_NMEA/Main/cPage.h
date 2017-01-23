// cPage.h

#ifndef _CPAGE_h
#define _CPAGE_h

void htmlPrint(String _str)
{
	Serial.println(_str);
	client.print(_str + "\n");
}

void printPageHeader()
{
	htmlPrint("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>");
	htmlPrint("<html xmlns=\"http://www.w3.org/1999/xhtml\">");
	htmlPrint("<head>");
	htmlPrint("<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />");
	htmlPrint("<meta name=\"generator\" content=\"Amaya, see http://www.w3.org/Amaya/\" />");
	htmlPrint("</head>");
	htmlPrint("<body style=\"background-color:#E5E5E5;margin-left:10em;margin-right:auto;\">");
	htmlPrint("<p style=\"font-size: 80pt\"><button style=\"font-size: 80pt\"></button></p>");
}

void printPageEnd()
{
	htmlPrint("</body>");
	htmlPrint("</html>");
}



#endif

