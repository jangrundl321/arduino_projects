// websocket server for sending message to client when touch sensor is used

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// set network and password
const char* ssid = "MagentaWLAN-GZF2";
const char* password = "23466747730399607166";

// variables for touch sensor
const int TOUCH_SENSOR_PIN = 5;
int lastStateOfTouchSensor = LOW;
int currentState = LOW;

// create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");




void setup() {
  Serial.begin(115200);  // Initialize serial
  
  pinMode(TOUCH_SENSOR_PIN, INPUT);

  initWebSocket();
  
 
}

void loop() {
  // Connect or reconnect to WiFi
  connectToSSID();
  readTouchSensor();
}

void connectToSSID(){
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
}

void readTouchSensor(){
  delay(1000);

  currentState = digitalRead(TOUCH_SENSOR_PIN);
  Serial.println(currentState);

  if((currentState == HIGH) && (lastStateOfTouchSensor == LOW)){
    Serial.println("The sensor is touched");
    messageClient();
  }
  
  // save the the last state
  lastStateOfTouchSensor = currentState;
  currentState = LOW;
}

void messageClient(){
  ws.textAll("Touch Sensor used");
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
 void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}