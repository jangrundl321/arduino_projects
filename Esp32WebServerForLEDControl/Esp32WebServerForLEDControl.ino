// simple server to turn two leds on\off

#include <WiFi.h>

// set network and password
const char* ssid = "add_ssid_of_network";
const char* password = "add_password_of_network";

// set digital output pins and their initial state
int ledRed = 4;
int ledBlue = 5;

char* ledRedState = "off";
char* ledBlueState = "off";

// set web server port number to 80
WiFiServer server(80);

// set variable for header of request
String header;

// needed to convert 'char[n] + char*' for Serial.print()
String str;


void setup() {
  Serial.begin(115200);
  
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  digitalWrite(ledRed, LOW);
  digitalWrite(ledBlue, LOW);

  // connect to wifi
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(". ");
  }
  
  // print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); 

  if(client) {
    Serial.println("New Client.");         
    String currentLine = "";              

    while(client.connected()) {
      if (client.available()) {           
        char c = client.read();            
        Serial.write(c);                   
        header += c;
        if (c == '\n') {       
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("red Led on");
              ledRedState = "on";
              digitalWrite(ledRed, HIGH);
            } 
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Red Led off");
              ledRedState = "off";
              digitalWrite(ledRed, LOW);
            } 
            else if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("Blue Led on");
              ledBlueState = "on";
              digitalWrite(ledBlue, HIGH);
            } 
            else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("Blue Led off");
              ledBlueState = "off";
              digitalWrite(ledBlue, LOW);
            }

            // display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // css settings
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server for LED-Control</h1>");

            // print current state of RED LED
            str = String("<p>RED LED - State ") + String(ledRedState) + String(" </p>?");
            //client.println("<p>RED LED - State " + ledRedState + "</p>");
            client.println(str);

            // display toggle button if RED LED is off
            if (ledRedState=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } 
            else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            // print current state of BLUE LED
            str = String("<p>BLUE LED - State ") + String(ledBlueState) + String(" </p>?");
            client.println(str);
            // display toggle button if BLUE LED is off
            if (ledBlueState=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } 
            else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            client.println("</body></html>");
            client.println();
            break;
        } 
        else{
          currentLine = "";
        }
      }
      else if (c != '\r'){
        currentLine += c;
      }
    }
  }
  // clear the header variable
  header = "";
  // close the connection
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
  }
}