// 08/04/15 Ported to ESP8266
/*--------------------------------------------------------------
  Program:      eth_websrv_page
  References:   - WebServer example by David A. Mellis and
                  modified by Tom Igoe
  Date:         7 January 2013
  Author:       W.A. Smith, http://startingelectronics.org
--------------------------------------------------------------*/
#include <Arduino.h>
#include <Streaming.h>
#include <ESP8266WiFi.h>

#define LOG0      lLineCount++ << " " << millis()
static long       lLineCount	= 0;      //Serial Monitor uses for clarity.

const char		*szSSID 						= "P291spot";
const char		*szPassword 				= "Qazqaz11";
const int 		sLedPin							= 2;

//Create an instance of the server and specify the port to listen on as an argument
WiFiServer server(80);

String szHTTP_Request;          // stores the HTTP request
boolean bLEDOn = false;   // state of LED, off by default

void setup()
{
	sSetupWiFi();
  pinMode(sLedPin, OUTPUT);       // LED on pin sLedPin
	return;
}	//setup


int sSetupWiFi() {
  Serial.begin(115200);
  Serial << LOG0 << " setup(): Begin" << endl;
  delay(10);

  //Connect to WiFi network
  Serial << LOG0 << " sesSetupWiFitup(): Connecting to "<< szSSID << endl;
  WiFi.begin(szSSID, szPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial << endl << LOG0 <<" setup(): WiFi connected"<< endl;

  //Start the server
  server.begin();
  Serial << LOG0 <<" setup(): Server started, my IP is  "<< WiFi.localIP() << endl;
	return 1;
}	//sSetupWiFi


void loop()
{
    WiFiClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        Serial << LOG0 <<" loop(): Got a Client"<< endl;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                szHTTP_Request += c;  // save the HTTP request 1 char at a time
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Arduino LED Control</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>FloJet LED</h1>");
                    client.println("<p>Click to switch LED on Arduino D2 on and off.</p>");
                    client.println("<form method=\"get\">");
                    ProcessCheckbox(client);
                    client.println("</form>");
                    client.println("</body>");
                    client.println("</html>");
                    //Serial.print(szHTTP_Request);
                    Serial << LOG0 <<"loop(): HTTP Request= |"<< szHTTP_Request <<"|"<< endl;
                    szHTTP_Request = "";    // finished with request, empty string
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                }
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}	//loop


// switch LED and send back HTML for LED checkbox
void ProcessCheckbox(WiFiClient cl)
{
    if (szHTTP_Request.indexOf("LED2=2") > -1) {  // see if checkbox was clicked
        // the checkbox was clicked, toggle the LED
        if (bLEDOn) {
            bLEDOn = 0;
        }
        else {
            bLEDOn = 1;
        }
    }

    if (bLEDOn) {    // switch LED on
        digitalWrite(sLedPin, HIGH);
        // checkbox is checked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\" checked>LED2");
    }
    else {              // switch LED off
        digitalWrite(sLedPin, LOW);
        // checkbox is unchecked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\">LED2");
    }
}	//ProcessCheckbox

/* void loop()
{
    WiFiClient client = server.available();  // try to get client
    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Arduino Web Page</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>Hello from FloJet!</h1>");
                    client.println("<p>A web page from Larry Beck's FloJet Arduino server</p>");
                    client.println("</body>");
                    client.println("</html>");
                    break;
                }	//if(c=='\n'&&...
                //Every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                }	//if(c=='\n')
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }	//elseif(c!='\r')
            } //if(client.available())
        } //while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } //if(client.available())
}	//loop*/
//Last line