#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const int ledPin[]={D0,D1,D2,D3,D4,D5,D6};
const char *ssid = "Kembo Home"; 
const char *password = "KemboHome"; 

const char *mqttBroker = "broker.hivemq.com";

const char *switchTopic = "led/switch/+";
const char *baseSwitchTopic = "led/switch/";

const char *toggleTopic = "led/toggle/+";
const char *baseToggleTopic = "led/toggle/";

const char *toggleFeedbackTopic="ledToggle/feedback";

const char *mqttUsername = "Lennox";
const char *mqttPassword = "password";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  for(int i=0;i<(sizeof(ledPin)/sizeof(ledPin[0]));i++){
    pinMode(ledPin[i],OUTPUT);
 }
  
  Serial.begin(9600);
  
  WiFi.begin(ssid,password);
  Serial.print("Connecting WiFi");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to the WiFi network");
  

  client.setServer(mqttBroker,mqttPort);
  client.setCallback(callback);
  
  reconnectBroker();
  
  client.subscribe(switchTopic);
  client.subscribe(toggleTopic);
  client.subscribe(toggleFeedbackTopic);
  }
 
void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    String message; 
    for (int i = 0; i < length; i++) {
        message += (char) payload[i]; 
    }
    
    Serial.println(message);
    
    if((String(topic) == (String (baseSwitchTopic)+="room1"))){
      digitalWrite(ledPin[1],message.toInt());
    }else if ((String(topic) == (String (baseSwitchTopic)+="room2"))){
      digitalWrite(ledPin[2],message.toInt());
    }else if ((String(topic) == (String (baseToggleTopic)+="room1"))){
      togglePublishLedFeedback(message.toInt(),1);
    }else if ((String(topic) == (String (baseToggleTopic)+="room2"))){
      togglePublishLedFeedback(message.toInt(),2);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void reconnectBroker(){
  while (!client.connected()) {
      String client_id = "nodeMCU-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqttUsername, mqttPassword)) {
          Serial.println("Public hiveMQ mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }}
}
void togglePublishLedFeedback(int times,int pin){
  char toggleMessage[100];
  char endingMessage[100];

  sprintf(toggleMessage, "starting toggling room %d", pin);
  client.publish(toggleFeedbackTopic, toggleMessage);
  for(int i=0;i<times;i++){
    digitalWrite(ledPin[pin], HIGH); 
    delay(500); 
    digitalWrite(ledPin[pin], LOW);
    delay(500); 
  }
  client.publish(toggleFeedbackTopic,endingMessage);
}

void loop() {
  
client.loop();
    delay(100);
    
}
