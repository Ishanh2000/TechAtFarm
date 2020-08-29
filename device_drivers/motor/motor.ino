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
void getMoisture();
String ipToStr(IPAddress ipa);
int motor_status[] = {0, 0, 0, 0};
void handle_login();
void handle_dashboard();
void handle_notFound();
String loginPage();
void generateToken();
String dashboard();

// global variables
String ssid = "yashvr";
String password = "yash2018";
String mac = "CC:50:E3:C6:8B:E9";
String taf = "ec2-52-15-97-130.us-east-2.compute.amazonaws.com";
//String taf = "192.168.137.114";
String sensor = "192.168.137.13";
int threshold_moisture = 45;
int required_moisture = 69;
int current_moisture = 0;
String token = "";
String public_ip = "";
// instanciate web server at port <port>
// This port is used for port forwarding.
int server_port = 80;
ESP8266WebServer server(server_port);

void setup() {
  pinMode(14, OUTPUT); // D5
  digitalWrite(14, HIGH);
  // pinMode(13, OUTPUT); // D7
  // digitalWrite(13, HIGH);
  // pinMode(12, OUTPUT); // D6
  // digitalWrite(12, HIGH);
  // pinMode(4, OUTPUT); // D2
  // digitalWrite(4, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600);
  Serial.println("\n--------------------Connected to Monitor--------------------");
  Serial.println("AUM SHREE GANESHAAYA NAMAH||");
  connectToWiFi(CONNECT);

  server.on("/", handle_login);
  server.on("/dashboard", handle_dashboard);
  server.onNotFound(handle_notFound);

  if (MDNS.begin("myland")) {
    Serial.println("mDNS servive has begun.");
  } else {
    Serial.println("mDNS service failed to establish.");
  }

  server.begin();
  MDNS.addService("http", "tcp", server_port);
}

void loop() {
  server.handleClient();
  // blink to display that device is live.
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  server.handleClient();
  if (WiFi.status() == WL_CONNECTED) {
    sendInfo();
  }
  server.handleClient();
  MDNS.update();
  server.handleClient();
  getMoisture();
  server.handleClient();
  if (current_moisture < threshold_moisture) {
    digitalWrite(14, LOW); // D5
    motor_status[0] = 1;
    // digitalWrite(13, LOW); // D7
    // motor_status[1] = 1;
    // digitalWrite(12, LOW); // D6
    // motor_status[2] = 1;
    // digitalWrite(4, LOW); // D2
    // motor_status[3] = 1;
  } else if (current_moisture >= required_moisture) {
    digitalWrite(14, HIGH); // D5
    motor_status[0] = 0;
    // digitalWrite(13, HIGH); // D7
    // motor_status[1] = 0;
    // digitalWrite(12, HIGH); // D6
    // motor_status[2] = 0;
    // digitalWrite(4, HIGH); // D2
    // motor_status[3] = 0;
  }
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
  public_ip = "";
  while (ch != '\r' && ch != '\n') {
    request += ch;
    public_ip += ch;
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
  request += "&local_ip=";
  request += ipToStr(WiFi.localIP());
  request += "&port=";
  request += (String)server_port;

  // attach host
  request += (String)" HTTP/1.1\r\n";
  request += (String)"Host: " + taf + "\r\n";
  Serial.println(request);
  // GET /dbms/device/?device_call=true&user_name=yashvr&password=yash2018&land_name=Sonalika&land_address=136+Block+C+Shyam+Nagar&district=Kanpur&public_ip=106.205.26.208&date_time=2019-07-06+17:29:18.389953&average_moisture=52&local_ip=192.168.137.22&port=80 HTTP/1.1
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

  response = client.readString(); // prints full response.
  if (response.indexOf("success") != -1) {
    Serial.println("Data sent to Tech At Farm.\n");
  }
  client.stop();

  // end process by turning led off.
  digitalWrite(LED_BUILTIN, HIGH);
}

void handle_login() {
  server.send(200, "text/html", loginPage());
}

void handle_dashboard() {
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == ssid && server.arg("password") == password) {
      if (server.method() == 2 && server.hasArg("token") && server.arg("token") == token) {
        server.send(200, "text/html", dashboard());
      } else {
        handle_notFound();
      }
    } else {
      handle_notFound();
    }
  } else {
    handle_notFound();
  }
}

void handle_notFound() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/html", loginPage());
}

String loginPage() {
  generateToken();
  String resp = "<!--AUM SHREE GANESHAAYA NAMAH||-->\n<!DOCTYPE html>\n<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n\t<head>\n\t\t<meta charset=\"utf-8\" />\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  resp += "\t\t<title>Device Manager Login</title>\n\t\t<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css\">\n\t\t<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js\"></script>\n";
  resp += "\t\t<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js\"></script>\n\t\t<style>\n\tbody {\n\t\t\t\tbackground-image: url(\"https://wallpapercave.com/wp/wp2474805.jpg\");\n\t\t\t\tbackground-color: #cccccc;\n";
  resp += "\t\t\t\tbackground-size: cover;\n\t\t\t}\n\t\t\t.container{\n\t\t\t\twidth: 28%;\n\t\t\t\tcolor: white;\n\t\t\t\tfont-family: Georgia;\n\t\t\t\tfont-weight: bold;\n\t\t\t\tbackground-color: rgb(165, 97, 42);\n";
  resp += "\t\t\t\tmargin-top: 10%;\n\t\t\t\tborder-radius: 27px;\n\t\t\t\topacity: 0.9;\n\t\t\t\tborder: 2px solid gray;\n\t\t\t}\n\t</style>\n\t</head>\n\t<body>";
  resp += "\n\t\t<center>\n\t\t\t<div class=\"container\">\n\t\t\t\t<h1>Device Manager</h1>\n\t\t\t\t<h3>Device Login</h3>\n\t\t\t\t<form class=\"form-inline\" action=\"http://" + ipToStr(WiFi.localIP()) + ":" + (String)server_port + "/dashboard\" method=\"POST\">\n";
  resp += "<input type=\"hidden\" name=\"token\" value=\"" + token + "\">";
  resp += "\t\t\t\t\t<div class=\"form-group\">\n\t\t\t\t\t\t<label>Username:</label>&ensp;\n\t\t\t\t\t\t<input type=\"text\" class=\"form-control\" name=\"username\" placeholder=\"Username\">\n\t\t\t\t\t</div>\n\t\t\t\t\t<br /><br />\n";
  resp += "\t\t\t\t\t<div class=\"form-group\">\n\t\t\t\t\t\t<label>Password:</label>&emsp;\n\t\t\t\t\t\t<input type=\"password\" class=\"form-control\" name=\"password\" placeholder=\"Password\">\n\t\t\t\t\t</div>\n";
  resp += "\t\t\t\t\t<br /><br />\n\t\t\t\t\t<div class=\"form-group\">\n\t\t\t\t\t\t<input type=\"submit\" class=\"form-control\" value=\"Submit\">\n\t\t\t\t\t</div>\n\t\t\t\t</form>\n\t\t\t\t<br />\n\t\t\t</div>\n\t\t</center>\n\t</body>\n</html>";
  return resp;
}

String ipToStr(IPAddress ipa) {
  return (String)(ipa[0]) + (String)"." + (String)(ipa[1]) + (String)"." + (String)(ipa[2]) + (String)"." + (String)(ipa[3]);
}

void generateToken() {
  token = "abcdefgh";
}

String dashboard() {
  String resp = "<!--AUM SHREE GANESHAAYA NAMAH||--><!--motor--><!DOCTYPE html><html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\"><head><meta charset=\"utf-8\" />";
  resp += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Device Manager Dashboard</title>";
  resp += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css\"><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js\"></script>";
  resp += "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js\"></script><style>";
  resp += "body {background-image: url(\"https://wallpapercave.com/wp/wp2474805.jpg\");background-color: #cccccc;background-size: cover;}";
  resp += ".container{width: 50%;font-family: Georgia;font-weight: bold;background-color: rgb(165, 97, 42);margin-top: 3%;border-radius: 27px;opacity: 0.9;border: 2px solid gray;}</style>";
  resp += "<script>function logout(){var form1 = document.getElementById(\"f1\");form1.submit();}function setTextWidth(){var list = document.getElementsByTagName(\"input\");for(var i=0;i<list.length;i++)";
  resp += "{var type = (list[i]).getAttribute(\"type\");if((type==\"text\")||(type==\"number\")){(list[i]).style.width = \"100%\";}}}</script></head><body onload=\"setTextWidth()\"><center><div class=\"container\">";
  resp += "<h1 style=\"color: white;\">Device Manager</h1><h3 style=\"color: white;\">Configuration Details</h3><table class=\"table\" style=\"border:3px gray solid;\">";

  for (int i = 0; i < 4; i++) {
    resp += "<tr class=\"active\"><td>Pump " + (String)(i + 1) + " Status:</td><td><input class=\"mutable\" type=\"radio\" value=\"on\" name=\"pumping"+(String)(i+1)+"\" checked=\"true\" disabled>&emsp;On&emsp;&emsp;&emsp;&emsp;";
    if (motor_status[i]) {
      resp += "<input class=\"mutable\" type=\"radio\" value=\"off\" name=\"pumping"+(String)(i+1)+"\" disabled>&emsp;Off";
    } else {
      resp += "<input class=\"mutable\" type=\"radio\" value=\"off\" name=\"pumping"+(String)(i+1)+"\" checked=\"true\" disabled>&emsp;Off";
    }
    resp += "</td></tr>";
  }

  resp += "<tr class=\"active\"><td>Username</td><td><input class=\"mutable form-control\" type=\"text\" value=\"yashvr\" name=\"farmer\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>Password</td><td><input class=\"form-control\" type=\"text\" value=\"yash2018\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>Land</td><td><input class=\"mutable form-control\" type=\"text\" value=\"Sonalika; 136 Block C Shyam Nagar; Kanpur\" name=\"land\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>Soil and Crop</td><td><input class=\"mutable form-control\" type=\"text\" value=\"Alluvial; Wheat\" name=\"soil_crop\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>Threshold Moisture</td><td><input class=\"mutable form-control\" type=\"number\" value=\"" + (String)threshold_moisture + "\" min=\"0\" max=\"100\" name=\"thr_mois\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>Required Moisture</td><td><input class=\"mutable form-control\" type=\"number\" value=\"" + (String)required_moisture + "\" min=\"0\" max=\"100\" name=\"req_mois\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>System ID</td><td><input class=\"mutable form-control\" type=\"text\" value=\"3223113311\" name=\"sys_id\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>MAC Address</td><td><input class=\"form-control\" type=\"text\" value=\"" + mac + "\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>Local IP</td><td><input class=\"mutable form-control\" type=\"text\" value=\"" + ipToStr(WiFi.localIP()) + "\" name=\"local_ip\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>Public IP</td><td><input class=\"form-control\" type=\"text\" value=\"" + public_ip + "\" readonly></td></tr>";
  resp += "<tr class=\"active\"><td>Port (Forwarding)</td><td><input class=\"mutable form-control\" type=\"text\" value=\"" + (String)server_port + "\" name=\"port\" readonly></td></tr></table>";
  resp += "<form action=\"http://" + ipToStr(WiFi.localIP()) + ":" + (String)server_port + "\" method=\"GET\" ><button class=\"btn btn-danger\" onclick=\"logout()\">LOGOUT</button>&emsp;</form><br />";
  resp += "<a href=\"https://tinyurl.com/techatfarm\" style=\"color:white;\">Visit Tech At Farm...</a><br /><br /></div><br /><br /></center></body></html>";
  return resp;
}
