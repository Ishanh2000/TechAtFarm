//AUM SHREE GANESHAAYA NAMAH||
// libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// definitions
#define RECONNECT 1
#define CONNECT 0

// functions
void connectToWiFi(byte reconnectToWiFi);
int getMoisture();

// global variables
String ssid = "ishanhpc";
String password = "JSRJHishanhpc";

void setup() {
  Serial.begin(9600);
  Serial.println("\n--------------------Connected to Monitor--------------------");
  Serial.println("AUM SHREE GANESHAAYA NAMAH||");
  connectToWiFi(CONNECT);
}

void loop() {
  getMoisture();
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

int getMoisture() {
  Serial.print("Attempting to find moisture.");
  WiFiClient client;
  while (!client.connect("192.168.137.223/", 80)) {
    Serial.print(".");
    delay(1000);
  }
  client.print("GET / HTTP/1.1\r\nHost: 192.168.137.223/\r\n\r\n");
  while (client.available() == 0) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  // Expected response
  //  HTTP/1.1 200 OK
  //  Content-Type: text/plain
  //  Content-Length: 4
  //  Connection: close
  //
  //  6.31



  String response = client.readString(); // stores full response.
  int index = response.indexOf("\r\n\r\n") + 4; // our ip starts 4 letters later.
  int moisture = 0;
  char ch = response[index];
  while ('0' <= ch && ch <= '9') {
    // ch must be between 0 and 9
    moisture = (moisture * 10) + ch - '0';
    ch = response[++index];
  }
  if (ch >= '5') {
    moisture++;
  }

  Serial.print("Moisture is: ");
  Serial.println(moisture);
  Serial.println();
  client.stop();
  return moisture;
}
