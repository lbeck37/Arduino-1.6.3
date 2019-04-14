// 4/13/19 https://github.com/esp8266/Arduino/issues/543
//Slider in HTML, CSS and Javascript

<div class="slidecontainer">
  <input type="range" min="1" max="100" value="50" class="slider" id="myRange">
</div>

.slidecontainer {
  width: 100%; /* Width of the outside container */
}

/* The slider itself */
.slider {
  -webkit-appearance: none;  /* Override default CSS styles */
  appearance: none;
  width: 100%; /* Full-width */
  height: 25px; /* Specified height */
  background: #d3d3d3; /* Grey background */
  outline: none; /* Remove outline */
  opacity: 0.7; /* Set transparency (for mouse-over effects on hover) */
  -webkit-transition: .2s; /* 0.2 seconds transition on hover */
  transition: opacity .2s;
}

/* Mouse-over effects */
.slider:hover {
  opacity: 1; /* Fully shown on mouse-over */
}

//Add CSS
/* The slider handle (use -webkit- (Chrome, Opera, Safari, Edge) and -moz- (Firefox) to override default look) */
.slider::-webkit-slider-thumb {
  -webkit-appearance: none; /* Override default look */
  appearance: none;
  width: 25px; /* Set a specific slider handle width */
  height: 25px; /* Slider handle height */
  background: #4CAF50; /* Green background */
  cursor: pointer; /* Cursor on hover */
}

.slider::-moz-range-thumb {
  width: 25px; /* Set a specific slider handle width */
  height: 25px; /* Slider handle height */
  background: #4CAF50; /* Green background */
  cursor: pointer; /* Cursor on hover */
}


//Add JavaScript
var slider = document.getElementById("myRange");
var output = document.getElementById("demo");
output.innerHTML = slider.value; // Display the default slider value

// Update the current slider value (each time you drag the slider handle)
slider.oninput = function() {
  output.innerHTML = this.value;
}


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define mot1PinA 16 // Brown A-IA  DO on nodemcu
#define mot1PinB 5  //Red  A -IB   D1 on nodemcu
#define mot2PinA 4  // orange  B - IA  = D2
#define mot2PinB 0  // yellow  B-IB = D3

const char* apssid = "Robot";
const char* appassword = "0123456789";
ESP8266WebServer server(80);
unsigned int mspeed = 900;
unsigned int lrspeed = 1023;
const char webpage[] = "<html>"\
"<head>"\

"<title>Robot Control</title>"\
"<style>"\
"div.container1 {"\
    "height: 10em;"\
   " position: relative }"\

"div.container1 p {"\
   " margin: 0;"\
    "//background: yellow;"\
   " position: absolute;"\
   " top: 50%;"\
   " text-align: center;"\
 " left: 50%;"\
  "  margin-right: -50%;"\
   " transform: translate(-50%, -50%) }"\
"</style>  "\

"<script>"\
"function showValue(newValue)"\
"{"\
"  document.getElementById(\"range\").innerHTML=newValue;"\
"}"\
"</script>"\
"</head>"\
"<body>"\
    "<h1>Carl Robot Control</h1><br><br><br>"\
    "<form class=\"f\" action=\"Forward\"> <input type=\"submit\" class=\"fb\" value=\"Forward\">  </form>" \
    "<form class=\"l\" action=\"Left\"> <input type=\"submit\" class=\"lb\" value=\"Left\"> </form>"\
    "<form class=\"s\" action=\"Stop\"> <input type=\"submit\" class=\"sb\" value=\"Stop\"> </form>"\
    "<form class=\"r\" action=\"Right\"><input type=\"submit\" class=\"rb\" value=\"Right\"> </form>"\
    "<form class=\"rev\"action=\"Reverse\"><input type=\"submit\" class=\"revb\" value=\"Reverse\"></form>"\
    "<form class=\"sf\" action=\"Forward\"> <input type=\"range\" min=\"0\" max=\"50\" value=\"25\" step=\"5\"onchange=\"this.form.submit()\"/></form>" \
"</body>"\

"</html>";

void handleRoot(){server.send (200, "text/html",webpage);}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void goRight(){
    Serial.println("Go right");
    analogWrite(mot1PinA, lrspeed);
    analogWrite(mot1PinB, 0);

    analogWrite(mot2PinA, 0);
    analogWrite(mot2PinB, lrspeed);
    handleRoot();
    delay(50);
    goStop();
}

void goLeft(){
    Serial.println("Go Left");
    analogWrite(mot1PinA, 0);
    analogWrite(mot1PinB, lrspeed);

    analogWrite(mot2PinA, lrspeed);
    analogWrite(mot2PinB, 0);
    handleRoot();
    delay(50);
    goStop();
}

void goForward(){
    Serial.println("Go Forward");
    analogWrite(mot1PinA, mspeed);
    analogWrite(mot1PinB, 0);

    analogWrite(mot2PinA, mspeed);
    analogWrite(mot2PinB, 0);
    handleRoot();
}

void goReverse(){
    Serial.println("Go Reverse");
    analogWrite(mot1PinA, 0);
    analogWrite(mot1PinB, mspeed);

    analogWrite(mot2PinA, 0);
    analogWrite(mot2PinB, mspeed);
    handleRoot();
}

void goStop(){
    Serial.println("Go Stop");
    analogWrite(mot1PinA, 0);
    analogWrite(mot1PinB, 0);

    analogWrite(mot2PinA, 0);
    analogWrite(mot2PinB, 0);
    handleRoot();
}

void setup(void){
  pinMode(mot1PinA, OUTPUT);
  pinMode(mot1PinB, OUTPUT);
  pinMode(mot2PinA, OUTPUT);
  pinMode(mot2PinB, OUTPUT);
  delay(1000);
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_AP);
  Serial.begin(115200);
  WiFi.softAP(apssid,appassword);
  Serial.println("");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/Forward", goForward);
  server.on("/Reverse", goReverse);
  server.on("/Left", goLeft);
  server.on("/Right", goRight);
  server.on("/Stop", goStop);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}