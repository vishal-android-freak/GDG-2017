//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseStream_ESP8266 is a sample that stream bitcoin price from a
// public Firebase and optionally display them on a OLED i2c screen.

#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
// Set these to run example.
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

void setup() {
  Serial.begin(115200);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  Firebase.begin("your_firebase_url", "your_database_secret_key");
  Firebase.stream("/automation");  
}


void loop() {
  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
  }
  
  if (Firebase.available()) {
     FirebaseObject event = Firebase.readEvent();
     String eventType = event.getString("type");
     eventType.toLowerCase();
     Serial.print(eventType);
     if (eventType == "put") {
      String path = event.getString("path");
      String data = event.getString("data");
      if (path.equals("/fan/value")) {
        if (data.equals("off")) {
          digitalWrite(D1, HIGH);
        } else {
          digitalWrite(D1, LOW);
        }
      } else if (path.equals("/light/value")) {
        if (data.equals("off")) {
          digitalWrite(D2, HIGH);
        } else {
          digitalWrite(D2, LOW);
        }
      }
     }
  }   
}
