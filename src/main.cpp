#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" // remember to add this file to the .gitignore

#define light_switch_1 21
#define light_switch_2 22
#define light_switch_3 23

bool blinkcounter[8][3] =
 {
  {false,false,false},
  {false,false,true},
  {false, true, false},
  {false, true, true},
  {true, false, false},
  {true,false, true},
  {true,true,false},
  {true,true,true}
  }; 

//Your Domain/IP Address name with URL route
const char * endpoint = "URI";

void setup() {

  pinMode(light_switch_1, OUTPUT);
  pinMode(light_switch_2, OUTPUT);
  pinMode(light_switch_2, OUTPUT);
  digitalWrite(light_switch_1,HIGH);
  digitalWrite(light_switch_2,LOW);
  digitalWrite(light_switch_1,LOW);
  Serial.begin(9600);
	
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WIFI_SSID, WIFI_PASS);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  
    // Establish a connection to the server
    http.begin(endpoint);
    
    // Specify content-type header
    http.addHeader("X-API-Key", KEYVALUE);
    http.addHeader("Content-type", "application-type");
    http.addHeader("Content-length", "76");
    //http.addHeader("Host", "https://ecse-three-led-api-v2.onrender.com");

    // Serialise JSON object into a string to be sent to the API
    StaticJsonDocument<64> doc;
    char * httpRequestData;
    
      for(int n = 0; n < 8; n++)
      {
        for(int i = 0; i < 3; i++)
        {
          doc["light_switch_1"] = blinkcounter[n][i];
          doc["light_switch_2"] = blinkcounter[n][i]; 
          doc["light_switch_3"] = blinkcounter[n][i];
        }
      }
     


    //doc["light_switch_1"] = digitalRead(light_switch_1);
    //doc["light_switch_2"] =  digitalRead(light_switch_2);
    //doc["light_switch_3"] =  digitalRead(light_switch_3);
		
		// convert JSON document, doc, to string and copies it into httpRequestData
    serializeJson(doc, httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    String http_response;

    // check reuslt of POST request. negative response code means server wasn't reached
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.print("HTTP Response from server: ");
      http_response = http.getString();
      Serial.println(http_response);

      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, http_response);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
   
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
      
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}



