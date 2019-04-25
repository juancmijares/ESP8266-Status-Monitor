#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "JCM2"
#define STAPSK  "embedded"
#define LED D0
#define BUTTON D1
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

// Define variables
String stmInput = "";
String serialMessage1 = "";
String serialMessage2 = "";
String serialMessage3 = "";
String serialMessage4 = "";
String serialMessage5 = "";
String serialMessage6 = "";
String serialMessage7 = "";
String serialMessage8 = "";
String serialMessage9 = "";
String serialMessage10 = "";
String htmlPage;

// Define other functions
void waitForNewData()
{
  while(1)
  {
    if (Serial.available() > 0)
    {
      break;
    }
  }
}

void getInput()
{
  waitForNewData();
  stmInput = Serial.readStringUntil('\0');  // detecting until \n worked, but trying \0 null character now
  // Serial.print("Serial input received: ");
  // Serial.println(stmInput);
  serialMessage1 = serialMessage2;
  serialMessage2 = serialMessage3;
  serialMessage3 = serialMessage4;
  serialMessage4 = serialMessage5;
  serialMessage5 = serialMessage6;
  serialMessage6 = serialMessage7;
  serialMessage7 = serialMessage8;
  serialMessage8 = serialMessage9;
  serialMessage9 = serialMessage10;
  serialMessage10 = stmInput;
  Serial.println(serialMessage1);
  Serial.println(serialMessage2);
  Serial.println(serialMessage3);
  Serial.println(serialMessage4);
  Serial.println(serialMessage5);
  Serial.println(serialMessage6);
  Serial.println(serialMessage7);
  Serial.println(serialMessage8);
  Serial.println(serialMessage9);
  Serial.println(serialMessage10);
}

void handleRefresh() {                          
  server.sendHeader("Location", "/");       
  server.send(303);                         
}

void buildHtmlPage(void)
{
  htmlPage = "<!DOCTYPE html>";
  htmlPage += "<html>";
  htmlPage += "<head>";                                        
  htmlPage += "<title>Dog Feeder Status Page</title>";
  htmlPage += "<meta http-equiv=\"refresh\" content=\"3\">";           
  htmlPage += "</head>";

  htmlPage +=  "<h1>Automatic Dog Feeder</h1>";
  
  
  htmlPage += "<h2>Status Monitor</h2>";
  htmlPage += "<form action=\"/REFRESH\" method=\"POST\"><input type=\"submit\" value=\"Refresh\"></form>";
  
  htmlPage += "<br>" + serialMessage1;
  htmlPage += "<br>" + serialMessage2;
  htmlPage += "<br>" + serialMessage3;
  htmlPage += "<br>" + serialMessage4;
  htmlPage += "<br>" + serialMessage5;
  htmlPage += "<br>" + serialMessage6;
  htmlPage += "<br>" + serialMessage7;
  htmlPage += "<br>" + serialMessage8;
  htmlPage += "<br>" + serialMessage9;
  htmlPage += "<br>" + serialMessage10;
  //htmlPage += "</textarea>";

  htmlPage += "</body>";
  htmlPage += "</html>";
}

void handleRoot()
{
  buildHtmlPage();
  server.send(200, "text/html", htmlPage);
}

void reloadPage()
{
  server.sendHeader("Location", "/");       
  server.send(303); 
}

void handleNotFound() {
  digitalWrite(LED, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(LED, 0);
}

void setup(void) {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  Serial.begin(115200);
  Serial.setTimeout(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/REFRESH", handleRefresh);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  getInput();
  server.handleClient();
  //MDNS.update();
}
