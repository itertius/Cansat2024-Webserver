#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
// #include <ESPAsyncWebServer.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Firebase_ESP_Client.h>

// webpage //
#include "webpage.h"
// ------------------------ //

// network //
const char* ssid = "kaidowsuknidnoy";
const char* pass = "numnumnumsupernum";
unsigned long previousMillis = 0;
unsigned long interval = 5000;
// ------------------------ //

// webserver port //
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
// ------------------------ //

// // mysql //
// IPAddress server_addr(127, 0, 0, 1);
// const int32_t server_port = 3306;
// const char* user = "root";
// const char* password = "iterrius";
// const char* database = "skybase";
// WiFiClient client;
// MySQL_Connection conn(&client);
// // ------------------------ //

// firebase //
#define API_KEY "AIzaSyAqoFAWt3NbaND-BM7RzhXCOv5AFD19S4k"
#define DATABASE_URL "https://cansat24-dfef7-default-rtdb.asia-southeast1.firebasedatabase.app"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;
bool signupOK = false;
String databasePath;
String BMPTemp = "/valueTemp";
String BMPPressure = "/valuePress";
String BMPAltitude = "/valueBMPAltitude";
String GYAccX = "/valueAccX";
String GYAccY = "/valueAccY";
String GYAccZ = "/valueAccZ";
String GYX = "/valueGyX";
String GYY = "/valueGyY";
String GYZ = "/valueGyZ";
String GPSLat = "/valueLat";
String GPSLng = "/valueLng";
String GPSAltitude = "/valueGPSAltitude";
String MCPTemp = "/valueMCPTemp";
// ------------------------ //

String JSONtxt;
String incoming = "";

// LoRa //
#define SS 15
#define RST 16
#define DIO0 2
// ------------------------ //

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Mock Update Data //
const int numData = 14;
String mockData[numData] = {
  // temp, pres,alt_bmp,accx,accy,accz,gx,gy,gz,lat,lng,altgps,temp_mcp,sec
  "23.5,1010,300,1.2,0.7,-0.5,25.4,18.2,-12.1,40.7128, -74.0060,310,22.8,1",
  "24.1,1011,302,1.0,0.3,-0.8,27.2,19.8,-10.7,51.5074, -0.1278,312,24.3,2",
  "23.5,1010,300,1.2,0.7,-0.5,25.4,18.2,-12.1,48.8566, 2.352,310,22.8,1",
  "24.1,1011,302,1.0,0.3,-0.8,27.2,19.8,-10.7,8.0863, 98.9063,312,24.3,2",
  "23.8,1012,301,0.8,0.5,-0.9,26.5,17.9,-11.4,14.3559, 100.5529,311,23.5,3",
  "18.7,1008,285,0.9,-0.2,-0.7,32.1,21.5,-9.8,12.9276, 100.8770,290,19.2,1",
  "19.4,1009,287,1.1,-0.4,-0.6,30.8,20.3,-11.1,17.0083, 99.8262,292,19.9,2",
  "19.1,1010,286,0.7,-0.1,-0.8,31.5,19.7,-10.4,12.5684, 99.9577,291,18.7,3",
  "21.2,1005,270,-0.3,0.8,-1.2,19.7,15.4,-8.5,9.5120, 100.0136,275,20.7,4",
  "21.8,1006,272,-0.5,0.6,-1.0,21.4,16.1,-9.2,19.3584, 98.4360,277,21.2,5",
  "21.5,1007,271,-0.7,0.4,-1.1,20.8,14.9,-8.8,7.7408, 98.778,276,20.8,6",
  "27.4,1015,325,1.5,0.9,-0.3,38.2,24.1,-7.9,14.0229, 99.5320,330,28.1,7",
  "28.0,1016,327,1.7,0.5,-0.4,36.9,23.7,-8.5,8.9167, 98.5236,332,28.7,8",
  "27.7,1017,326,1.3,0.7,-0.6,37.5,24.4,-8.2,7.6244, 99.0792,331,27.4,9"
};
// ------------------------ //

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void setup() {
  Serial.begin(9600);

  while (!Serial);

  // init WiFi //
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  // ------------------------ //

  // // connect to mysql //
  // char user_char[20];
  // char password_char[20];
  // strcpy(user_char, user);
  // strcpy(password_char, password);
  // if (conn.connect(server_addr, server_port, user_char, password_char)) {
  //   Serial.println("connected");
  // } else {
  //   Serial.println("connection failed");
  // }
  // // ------------------------ //

  // // init LoRa //
  // LoRa.setPins(SS, RST, DIO0);
  // if (!LoRa.begin(433E6)) {
  //   Serial.println("LoRa Error");
  //   while (1);
  // }

  // init firebase //
  config.api_key = API_KEY; // assign api key
  config.database_url = DATABASE_URL; // assign realtime database
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println(" - Firebase OK - ");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize Server //
  server.on("/", handleRoot);
  server.begin();
  webSocket.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  // int packetSize = LoRa.parsePacket();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
  }
 
  if (true) { // if (packetSize) {}

    // Read the packet //
    // while (LoRa.available()) {
    //   incoming += LoRa.readString();
    // }

    // Mock Update Data //
    int dataIdx = (currentMillis / interval) % numData;
    incoming = mockData[dataIdx];
    // Serial.println("Current data index: " + String(dataIdx));
    // Serial.println("Incoming data: " + incoming);
    // ------------------------ //

    // case if sensor data is csv format //
    // BMP280 == temp, pressure, altitude
    // GY521 == Acceleration X Y Z and Gyro X Y Z
    // gps == lat, lng, altitude
    // mcp == temp

    // BMP280 == valueTemp, valuePress, valueBMPAltitude
    String valueTemp = getValue(incoming, ',', 0);
    String valuePress = getValue(incoming, ',', 1);
    String valueBMPAltitude = getValue(incoming, ',', 2);

    // GY521 == acceleration = valueAccX, valueAccY, valueAccZ | Gyroscope = valueGyX, valueGyY, valueGyZ
    // Acc //
    String valueAccX = getValue(incoming, ',', 3);
    String valueAccY = getValue(incoming, ',', 4);
    String valueAccZ = getValue(incoming, ',', 5);
    // Gyro //
    String valueGyX = getValue(incoming, ',', 6);
    String valueGyY = getValue(incoming, ',', 7);
    String valueGyZ = getValue(incoming, ',', 8);

    // GPS == valueLat, valueLng, valueGPSAltitude
    String valueLat = getValue(incoming, ',', 9);
    String valueLng = getValue(incoming, ',', 10);
    String valueGPSAltitude = getValue(incoming, ',', 11);

    // MCP9808 == temp
    String valueMCPTemp = getValue(incoming, ',', 12);

    String valueSyncSec = getValue(incoming, ',', 13);
    // ------------------------ //

    webSocket.loop();
    server.handleClient();

    // JSON Data //
    JSONtxt = "{\"valueTemp\":\"" + valueTemp + "\",";
    JSONtxt += "\"valuePress\":\"" + valuePress + "\",";
    JSONtxt += "\"valueBMPAltitude\":\"" + valueBMPAltitude + "\",";
    JSONtxt += "\"valueAccX\":\"" + valueAccX + "\",";
    JSONtxt += "\"valueAccY\":\"" + valueAccY + "\",";
    JSONtxt += "\"valueAccZ\":\"" + valueAccZ + "\",";
    JSONtxt += "\"valueGyX\":\"" + valueGyX + "\",";
    JSONtxt += "\"valueGyY\":\"" + valueGyY + "\",";
    JSONtxt += "\"valueGyZ\":\"" + valueGyY + "\",";
    JSONtxt += "\"valueLat\":\"" + valueLat + "\",";
    JSONtxt += "\"valueLng\":\"" + valueLng + "\",";
    JSONtxt += "\"valueGPSAltitude\":\"" + valueGPSAltitude + "\",";
    JSONtxt += "\"valueMCPTemp\":\"" + valueMCPTemp + "\",";
    JSONtxt += "\"valueSyncSec\":\"" + valueSyncSec + "\"}";

    webSocket.broadcastTXT(JSONtxt);
    // Serial.println(JSONtxt);
    
    // firebase //
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) { // set store freq
      sendDataPrevMillis = millis();
      databasePath = valueSyncSec + "/";
      json.set(BMPTemp.c_str(), valueTemp);
      json.set(BMPPressure.c_str(), valuePress);
      json.set(BMPAltitude.c_str(), valueBMPAltitude);
      json.set(GYAccX.c_str(), valueAccX);
      json.set(GYAccY.c_str(), valueAccY);
      json.set(GYAccZ.c_str(), valueAccZ);
      json.set(GYX.c_str(), valueGyX);
      json.set(GYY.c_str(), valueGyY);
      json.set(GYZ.c_str(), valueGyZ);
      json.set(GPSLat.c_str(), valueLat);
      json.set(GPSLng.c_str(), valueLng);
      json.set(GPSAltitude.c_str(), valueGPSAltitude);
      json.set(MCPTemp.c_str(), valueMCPTemp);
      Serial.printf(" - Set json... %s - \n", Firebase.RTDB.setJSON(&fbdo, databasePath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
    }
    // ------------------------ //

    incoming = "";
  }

}
