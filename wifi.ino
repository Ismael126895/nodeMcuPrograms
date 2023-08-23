#include "ESP8266WiFi.h"

void setup() {
  Serial.begin(9600);
  delay(1000);

  char ssid[50];
  char password[50];
  while (true){
    Serial.print("Enter WiFi Name: ");
  while (Serial.available() == 0) {
    
  }
  int bytesRead = Serial.readBytesUntil('\n', ssid, sizeof(ssid)); 
  ssid[bytesRead] = '\0';
  Serial.print("You have entered WiFi name: ");
  Serial.println(ssid);

  Serial.print("Enter WiFi password: ");
  while (Serial.available() == 0) {
 
  }
  bytesRead = Serial.readBytesUntil('\n', password, sizeof(password)); 
  password[bytesRead] = '\0';
   Serial.print("You have entered WiFi password: ");
  Serial.println(password);

  WiFi.begin(ssid, password);

  unsigned long startTime = millis();
  Serial.print("Connecting To WiFi Network");
  while (WiFi.status() != WL_CONNECTED && millis()-startTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    Serial.println("IP address: " + WiFi.localIP().toString());
    Serial.print("You are connected to: ");
    Serial.println(ssid);
    break;
  } else {
    Serial.println("\nWiFi connection failed within timeout");
    Serial.println("Please try again or connect to a new network");
    continue;
  }
    }
  
}

void loop() {
  // Nothing
}
