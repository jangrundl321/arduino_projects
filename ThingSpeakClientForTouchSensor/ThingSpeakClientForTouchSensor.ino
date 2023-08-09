#include <WiFi.h>
#include "ThingSpeak.h"

// set network and password
const char* ssid = "MagentaWLAN-GZF2";
const char* password = "23466747730399607166";

WiFiClient  client;

long unsigned int myChannelNumber = 2145747;
const char * myWriteAPIKey = "NWV37OXCHXPNJ6ZA";
const int channelField = 1;

// variables for touch sensor
const int TOUCH_SENSOR_PIN = 5;
int lastStateOfTouchSensor = LOW;
int currentState = LOW;

void setup() {
  Serial.begin(115200);  // Initialize serial
  
  pinMode(TOUCH_SENSOR_PIN, INPUT);

  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password); 
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  readTouchSensor();
}

void readTouchSensor(){
  delay(1000);

  currentState = digitalRead(TOUCH_SENSOR_PIN);
  Serial.println(currentState);


  if((currentState == HIGH) && (lastStateOfTouchSensor == LOW)){
    Serial.println("The sensor is touched");
    writeToThingSpeakChannel(currentState);
  }
  
  // save the the last state
  lastStateOfTouchSensor = currentState;
  currentState = LOW;
}

void writeToThingSpeakChannel(int sensorValue){
  int x = ThingSpeak.writeField(myChannelNumber, channelField, sensorValue, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}