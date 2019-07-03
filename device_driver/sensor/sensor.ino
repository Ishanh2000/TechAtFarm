//AUM SHREE GANESHAAYA NAMAH||
// libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// definitions
#define RECONNECT 1
#define CONNECT 0

// functions
void connectToWiFi(byte reconnectToWiFi);
float getMoisture();
void handle_sendData();
void handle_sendData();


// global variables
String ssid = "ishanhpc";
String password = "JSRJHishanhpc";
int max_val = 728;
int min_val = 265;
int iterations = 100;

// instanciaion of server
ESP8266WebServer server(80);

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial.println("\n--------------------Connected to Monitor--------------------");
  Serial.println("AUM SHREE GANESHAAYA NAMAH||");

  connectToWiFi(CONNECT);
  server.on("/", handle_sendData);
  server.onNotFound(handle_notFound);
  server.begin();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    connectToWiFi(CONNECT);
  }
  server.handleClient();
}

void connectToWiFi(byte reconnectToWiFi) {
  if ((WiFi.status() == WL_CONNECTED) && (reconnectToWiFi == 0)) {
    Serial.println("Connected to WiFi already.");
    return;
  }

  int time_elapsed = 0;
  Serial.print((String)"Attempting connection with WiFi network \"" + ssid + "\".");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    time_elapsed++;
    if (time_elapsed >= 60) {
      // WiFi network searching time threshold set to 60 seconds.
      time_elapsed = 0;
      Serial.println("\nSorry, WiFi connection taking too long.\nReattempting in 10 seconds.\n");
      // delay for ten seconds
      delay(10000);
      Serial.print("Reconnecting.");
    }
  }
  // wifi would be connected upto this point
  Serial.print((String)"\nConnected to WiFi network \"" + ssid + "\". Local IP Address: ");
  Serial.println(WiFi.localIP());
}

float getMoisture() {
  int val = 0;
  for (int i = 0; i < iterations; i++) {
    val += analogRead(A0);
  }
  val /= iterations;
  float moisture = ((float)(max_val - val)) / ((float)(max_val - min_val));
  moisture *= 100;
  return moisture;
}

void handle_sendData(){
  server.send(200, "text/plain", (String)(getMoisture()));
}

void handle_notFound(){
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", (String)(getMoisture()));
}
