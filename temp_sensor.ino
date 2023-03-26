#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "dht.h"

ESP8266WebServer server(80);            // create webserver object to listen for HTTP requests on port 80


#define dht_apin 2 // Analog Pin sensor is connected to
dht DHT;                       // create i2c temperature/humidity sensor object

 char ssid[]     = "Adesh";         // wifi credentials
 char password[] = "7028ADMIN99";

const int  relayPin = D1;

String      htmlPage;                   // webpage text to be sent out by server when main page is accessed

void setup() {
  Serial.begin(9600);

  digitalWrite(relayPin, LOW);         // relay is not energized initially (active low)
  pinMode(relayPin, OUTPUT);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();                    // disconnect if previously connected
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // functions to call when client requests are received
  server.on("/", handleRoot);     
  server.on("/RELAYON", handleRelayOn);
  server.on("/RELAYOFF", handleRelayOff);
  server.onNotFound(handleNotFound);        

  server.begin();                           // start web server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                    // listen for HTTP requests from clients
  DHT.read11(dht_apin);
  delay(100);
}


// client request handling functions

// send main web page when ip+"/" is accessed
void handleRoot() {
  buildHtmlPage();
  server.send(200, "text/html", htmlPage);
}

// turn relay on and redirect to main page
void handleRelayOn() {                          
  digitalWrite(relayPin, LOW);     
  server.sendHeader("Location", "/");       
  server.send(303);                         
}

// turn relay off and redirect to main page
void handleRelayOff() {                         
  digitalWrite(relayPin, HIGH);     
  server.sendHeader("Location", "/");       
  server.send(303);                         
}

// send HTTP status 404: Not Found when there's no handler for the client request
void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); 
}

// create the html page for the root path of the web server
void buildHtmlPage() {
  htmlPage = "<!DOCTYPE html>";
  htmlPage += "<html>";
  htmlPage += "<head>";                                       
  htmlPage += "<title>ESP8266 Web Server</title>";             
  htmlPage += "<meta http-equiv=\"refresh\" content=\"10\">";  // auto-refresh the page every 10 seconds to show new data
  htmlPage += "<meta charset=\"UTF-8\">";                      // UTF-8 allows the degree symbol to display correctly
  htmlPage += "</head>";
  
  htmlPage += "<BODY bgcolor='#E0E0D0'>";                      // body section, set background color

  // retrieve temperature/humidity from sensor to display on main page
  
  htmlPage += "<br>Temperature °C: " + String(DHT.temperature);
  htmlPage += "<br>Humidity   %RH: " + String(DHT.humidity);
  htmlPage += "<br>";

 
  // show relay status and action buttons
  String relayState = ((!digitalRead(relayPin)) ? "on" : "off");
  htmlPage += "<br>Relay: " + relayState;
  htmlPage += "<form action=\"/RELAYON\" method=\"POST\"><input type=\"submit\" value=\"Relay On\"></form>";
  htmlPage += "<form action=\"/RELAYOFF\" method=\"POST\"><input type=\"submit\" value=\"Relay Off\"></form>";
  

  htmlPage += "</body>";
  htmlPage += "</html>";
}
