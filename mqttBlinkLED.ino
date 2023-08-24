#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define ledPin D1

const char *ssid = "Kembo Home"; 
const char *password = "KemboHome"; 

const char *mqtt_broker = "broker.hivemq.com";
const char *switchTopic = "led/switch";
const char *toggleTopic ="led/toggle";
const char *statusTopic="led/status";
const char *switchFeedbackTopic="ledSwitch/feedback";
const char *toggleFeedbackTopic="ledToggle/feedback";

const char *mqtt_username = "Lennox";
const char *mqtt_password = "password";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);
  
  WiFi.begin(ssid,password);
  Serial.print("Connecting WiFi");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to the WiFi network");
  

  client.setServer(mqtt_broker,mqtt_port);
  client.setCallback(callback);
  reconnectBroker();
  client.publish(switchTopic, "LED switching");
  client.subscribe(switchTopic);
  client.publish(toggleTopic, "LED toggling");
  client.subscribe(toggleTopic);
  client.subscribe(statusTopic);
  }
 
void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    String message;
    for (int i = 0; i < length; i++) {
        message += (char) payload[i];  // Convert *byte to string
    }
    Serial.print(message);
    
    if(String(topic)==switchTopic){
      switchPubLEDFeedback(message);
    }else if (String(topic)==toggleTopic){
      togglePubLEDFeedback(message.toInt());
    }else if (message=="query"){
      pubLEDStatus();
    }
    Serial.println();
    Serial.println("-----------------------");
}

void switchPubLEDFeedback(String state){

  if (state == "on") {
      digitalWrite(ledPin, HIGH); 
      ledSwitchFeedback(digitalRead(ledPin));
      delay(2000);
      Serial.print("The LED is ON");
    } else if (state == "off") {
      digitalWrite(ledPin, LOW); 
      ledSwitchFeedback(digitalRead(ledPin));
      Serial.print("The LED is off");
    }
}
void ledSwitchFeedback(int ledState){
  if (ledState == HIGH) {
    client.publish(switchFeedbackTopic, "on");
  } else if (ledState == LOW){
    client.publish(switchFeedbackTopic, "off");
  }
}
void togglePubLEDFeedback(int times){
  Serial.println("\nStarting to toggle");
  client.publish(toggleFeedbackTopic,"starting toggling");
  for(int i=0;i<times;i++){
    digitalWrite(ledPin, HIGH); 
    delay(500); 
    digitalWrite(ledPin, LOW);
    delay(500); 
  }
  client.publish(toggleFeedbackTopic,"ended toggling");
}
void reconnectBroker(){
  while (!client.connected()) {
      String client_id = "nodeMCU-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public hiveMQ mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }}
}
void pubLEDStatus(){
  int ledState=digitalRead(ledPin);
  if(ledState == HIGH){
    client.publish(statusTopic,"I am on");
  }else if (ledState == LOW){
    client.publish(statusTopic,"I am off");
  }
}
void loop() {
  
client.loop();
    delay(100);
    
}
