/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//#include external libraries
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4

const char *ssid = "hidden"; //YourSSIDHere
const char *password = "polis12345"; //YourPSKHere

WebServer server(80);

//#can change to other GPIO as long not ADC2 if in WiFi mode
const int led = 13;

//#######################################################################
void handleRoot() {
  digitalWrite(led, 1);
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,

    "<html>\ TEST <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 with DHT11</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
    </head>\
    <body>\
      <h1>ESP32 with DHT11 as webserver</h1>\
      <p>Uptime: %02d:%02d:%02d</p>\
      <p>Temperature: %02d&#8451; Humidity: %02d&#37;</p>\
    </body>\
    </html>",

           hr, min % 60, sec % 60, DHT.temperature, DHT.humidity
          );
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

//#######################################################################
void handleNotFound() {
  digitalWrite(led, 1);
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
  digitalWrite(led, 0);
}

//#######################################################################
 server.on("/", handleRoot);
  //# type at browser -> esp32.local/inline 
  //#will produce this
  server.on("/inline", []() { 
    server.send(200, "text/plain", "this works as well");
  });
  
//#######################################################################
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

//#######################################################################

  void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
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

  //enabling DMS service
  //#can change to other than esp32
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  
//#######################################################################
 //# type at browser -> esp32.local
  //will produce this
void loop(void) {
  
  //#Read data from DHT11
  int chk = DHT.read(DHT11_PIN);    
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
  
  //#Call HTML function
  server.handleClient();
  
  delay(10000);
  
}
