//AUM SHREE GANESHAAYA NAMAH||
// libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// definitions
#define RECONNECT 1
#define CONNECT 0

// functions
void connectToWiFi(byte reconnectToWiFi);
void getMoisture();

// global variables
String ssid = "yashvr";
String password = "yash2018";
String taf = "ec2-52-15-97-130.us-east-2.compute.amazonaws.com";
String sensor = "192.168.137.225";
int threshold_moisture = 45;
int required_moisture = 69;
int current_moisture = 0;

void setup() {
  pinMode(14, OUTPUT); // D5
  digitalWrite(14, HIGH);
  pinMode(13, OUTPUT); // D7
  digitalWrite(13, HIGH);
  pinMode(12, OUTPUT); // D6
  digitalWrite(12, HIGH);
  pinMode(4, OUTPUT); // D2
  digitalWrite(4, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);
  Serial.println("\n--------------------Connected to Monitor--------------------");
  Serial.println("AUM SHREE GANESHAAYA NAMAH||");
  connectToWiFi(CONNECT);
}

void loop() {
  // blink to display that device is live.
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  getMoisture();
  if (current_moisture < threshold_moisture) {
    digitalWrite(14, LOW); // D5
    digitalWrite(13, LOW); // D7
    digitalWrite(12, LOW); // D6
    digitalWrite(4, LOW); // D2
  } else if (current_moisture >= required_moisture) {
    digitalWrite(14, HIGH); // D5
    digitalWrite(13, HIGH); // D7
    digitalWrite(12, HIGH); // D6
    digitalWrite(4, HIGH); // D2
  }
  if (WiFi.status() == WL_CONNECTED) {
    sendInfo();
  }
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

void getMoisture() {
  Serial.print("Attempting to find moisture.");
  WiFiClient client;
  while (!client.connect(sensor, 80)) {
    Serial.print(".");
    delay(1000);
  }
  client.print((String)"GET /?username=yashvr&password=yash2018 HTTP/1.1\r\nHost: " + sensor + "\r\n\r\n");
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
  current_moisture = 0;
  char ch = response[index];
  while ('0' <= ch && ch <= '9') {
    // ch must be between 0 and 9
    current_moisture = (current_moisture * 10) + ch - '0';
    ch = response[++index];
  }
  if (ch >= '5') {
    current_moisture++;
  }

  Serial.print("Moisture is: ");
  Serial.println(current_moisture);
  Serial.println();
  client.stop();
}

void sendInfo() {
  digitalWrite(LED_BUILTIN, LOW);
  WiFiClient client;
  while (!client.connect("worldtimeapi.org", 80)) {
    Serial.print(".");
    delay(1000);
  }
  client.print("GET /api/ip.txt HTTP/1.1\r\nHost: worldtimeapi.org\r\n\r\n");
  while (client.available() == 0) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  // Excpected Output
  //  HTTP/1.1 200 OK
  //  Connection: keep-alive
  //  Server: Cowboy
  //  Date: Sat, 06 Jul 2019 08:05:19 GMT
  //  Content-Length: 308
  //  Content-Type: text/plain; charset=utf-8
  //  Cache-Control: max-age=0, private, must-revalidate
  //  Access-Control-Allow-Origin: *
  //  Access-Control-Expose-Headers:
  //  Access-Control-Allow-Credentials: true
  //  Via: 1.1 vegur
  //
  //  abbreviation: IST
  //  client_ip: 14.139.38.153
  //  datetime: 2019-07-06T13:35:20.913310+05:30
  //  day_of_week: 6
  //  day_of_year: 187
  //  dst: false
  //  dst_from:
  //  dst_offset: 0
  //  dst_until:
  //  raw_offset: 19800
  //  timezone: Asia/Kolkata
  //  unixtime: 1562400320
  //  utc_datetime: 2019-07-06T08:05:20.91331+00:00
  //  utc_offset: +05:30
  //  week_number: 27

  String response = client.readString(); // stores full response.
  client.stop();

  // public_ip, moisture and date_time (IST) required
  // http://<taf>/dbms/device/?device_call=true&user_name=yashvr&password=yash2018&land_name=Sonalika&land_address=136+Block+C+Shyam+Nagar&district=Kochi&public_ip=9.22.3.6&date_time=2019-07-07+12:36:00%2B05:30&average_moisture=89

  String request = "GET /dbms/device/?";
  request += "device_call=true";
  request += "&user_name=yashvr";
  request += "&password=yash2018";
  request += "&land_name=Sonalika";
  request += "&land_address=136+Block+C+Shyam+Nagar";
  request += "&district=Kanpur";

  // get the public_ip
  int index = response.indexOf("client_ip: ") + 11; // our ip starts 4 letters later.
  char ch = response[index];
  request += "&public_ip=";
  while (ch != '\r' && ch != '\n') {
    request += ch;
    ch = response[++index];
  }

  // get the date and time in IST
  index = response.indexOf("datetime: ") + 10;
  ch = response[index];
  request += "&date_time=";
  while (ch != '+') {
    request += (ch == 'T') ? '+' : ch;
    ch = response[++index];
  }

  // get the average moisture
  request += "&average_moisture=";
  request += (String)current_moisture;

  // attach host
  request += (String)" HTTP/1.1\r\n";
  request += (String)"Host: " + taf + "\r\n";
  Serial.println(request);
  // GET /dbms/device/?device_call=true&user_name=yashvr&password=yash2018&land_name=Sonalika&land_address=136+Block+C+Shyam+Nagar&district=Kanpur&public_ip=106.205.26.208&date_time=2019-07-06+17:29:18.389953&average_moisture=52 HTTP/1.1
  // Host: <taf>

  Serial.println("Attempting to connect to Tech At Farm...");
  //  WiFiClient client1;
  while (!client.connect(taf, 80)) {
    Serial.print(".");
    delay(1000);
  }

  client.println(request);
  while (client.available() == 0) {
    Serial.print("*");
    delay(1000);
  }
  Serial.println();

  // Excpected Output
  // ?

  Serial.println(client.readString()); // prints full response.
  client.stop();

  // end process by turning led off.
  digitalWrite(LED_BUILTIN, HIGH);
}
