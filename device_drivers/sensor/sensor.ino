//AUM SHREE GANESHAAYA NAMAH||
// libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include<ESP8266mDNS.h>

// definitions
#define RECONNECT 1
#define CONNECT 0

// functions
void connectToWiFi(byte reconnectToWiFi);
float getMoisture();
void handle_sendData();
void handle_notFound();

// global variables
String ssid = "yashvr";
String password = "yash2018";
int max_val = 750;
int min_val = 265;
int iterations = 100;
int server_port = 80;
int blink_time = 0; // threshold for blink time

// instanciaion of server
ESP8266WebServer server(server_port);

void setup() {
  pinMode(A0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("\n--------------------Connected to Monitor--------------------");
  Serial.println("AUM SHREE GANESHAAYA NAMAH||");

  connectToWiFi(CONNECT);
  server.on("/", handle_sendData);
  server.onNotFound(handle_notFound);
  if (MDNS.begin("sensor")) {
    Serial.println("mDNS service has begun.");
  } else {
    Serial.println("mDNS service could not be established.");
  }
  server.begin();
  MDNS.addService("http", "tcp", server_port);
}

void loop() {
  blink_time++;
  delay(1);
  if (blink_time >= 5000) {
    // blibk every 5 seconds to show device is alive.
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    blink_time = 0;
  }
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi(CONNECT);
  }
  MDNS.update();
  server.handleClient();
}

void connectToWiFi(byte reconnectToWiFi) {
  digitalWrite(LED_BUILTIN, LOW);
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
  digitalWrite(LED_BUILTIN, HIGH);
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

void handle_sendData() {
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == ssid && server.arg("password") == password) {
      server.send(200, "text/plain", (String)(getMoisture()));
    } else {
      server.send(200, "text/plain", "Method not allowed.");
    }
  } else {
    server.send(200, "text/plain", "Method not allowed.");
  }
}

void handle_notFound() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "Method not allowed.");
}
