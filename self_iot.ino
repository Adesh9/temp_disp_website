
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "index.h"  //Web page header file

WebServer server(80);

//Enter your SSID and PASSWORD
const char* ssid = "Adesh";
const char* password = "7028ADMIN99";


void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {
 int a = analogRead(A0);
 String adcValue = String(a);
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}



void setup(void){
  Serial.begin(9600);
  Serial.println();
  Serial.println("Booting Sketch...");



  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

 
  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}


void loop(void){
  server.handleClient();
  delay(1);
}
