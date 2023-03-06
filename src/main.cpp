#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" // remember to add this file to the .gitignore

#define light_switch_1 23
#define light_switch_2 22
#define light_switch_3 21


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
const char * endpoint = URI;

void setup() {

  pinMode(light_switch_1, OUTPUT);
  pinMode(light_switch_2, OUTPUT);
  pinMode(light_switch_2, OUTPUT);

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
   

      for(int n = 0; n < 8; n++)
      {
        http.addHeader("X-API-Key", KEYVALUE);
        http.addHeader("Content-type", "application/json");
          //http.addHeader("Host", "https://ecse-three-led-api-v2.onrender.com");

          // Serialise JSON object into a string to be sent to the API
        StaticJsonDocument<72> doc;
        String httpRequestData;


        digitalWrite(light_switch_1, blinkcounter[n][0]);
        delay(2000);
        digitalWrite(light_switch_2, blinkcounter[n][1]);
        delay(2000);
        digitalWrite(light_switch_3, blinkcounter[n][2]);
        delay(2000);


        doc["light_switch_1"] = blinkcounter[n][0]; 
        doc["light_switch_2"] = blinkcounter[n][1];
        doc["light_switch_3"] = blinkcounter[n][2];

        serializeJson(doc, httpRequestData);

         // Send HTTP POST request
        int httpResponseCode = http.PUT(httpRequestData);

        String http_response;
      // check reuslt of POST request. negative response code means server wasn't reached
          if (httpResponseCode > 0) 
          {
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
          else
          {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
          }

          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);

          http.end();
      
        }
      }     
   
      }
      else {
      Serial.println("WiFi Disconnected");
    }
}



