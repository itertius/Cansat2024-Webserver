#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Firebase_ESP_Client.h>

// webpage //
const char mainpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sky's Ace</title>
    <style>
@import url("https://fonts.googleapis.com/css2?family=Poppins:wght@400;500;600;700;800;900&display=swap");

* {
  font-family: 'Poppins', sans-serif;
  font-weight: 600;
}

.fullscreen {
  height: 150vh;
  overflow: auto;
  background-color: black;
}

header,
footer {
  display: flex;
  justify-content: center;
  align-items: center;
  font-size: x-large;
  height: 10%;
  background-color: whitesmoke;
}

main {
  background-color: rgb(12, 12, 21);
  height: 50%;
  color: white;
  display: grid;
  grid-template-columns: 1fr 1fr;
  padding: 1vh 0 3vh 0;
}

.sensor-data {
  display: grid;
  grid-template-columns: 1fr 1fr;
}

#map,
#temp-graph,
#press-graph,
#gyro-anime,
#rocket-path {
  width: 100%;
  background-color: #ffffff;
}

#map {
  height: 100%;
}

#temp-graph,
#press-graph,
#gyro-anime {
  height: 40vh;
}

#rocket-path {
  height: 98%;
}

h2,
.footer {
  text-align: center;
}

#cansat_phase,
.phase,
#Launch,
#Deploy,
#Land {
  color: white;
}

.start-btn,
#choose {
  display: block;
  margin: 0 auto;
}

.start-btn {
  width: 5vw;
  height: 5vh;
  font-size: large;
}

#choose {
  width: 10vw;
  height: 5vh;
  font-size: large;
}

sub {
  height: 40%;
  padding: 1vh 0;
}

.visual-data {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  column-gap: 4px;
  justify-content: center;
  text-align: center;
  color: blue;
  padding: 3vh 1vw 1vh 1vw;
}

.cube-content {
  padding-top: 2vh;
  height: 25%;
}

.phasephase {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  justify-content: center;
  text-align: center;
}

@media screen and (max-width: 1200px) {
  .main {
    padding: 6px;
  }
}

@media screen and (max-width: 768px) {
  main {
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    grid-auto-rows: minmax(auto, 1fr);
    width: 100%;
    height: 100vh;
  }

  .visual-data {
    display: inline-block;
  }

  .start-btn {
    width: 18vw;
    height: 5vh;
    font-size: large;
  }

  #choose {
    width: 30vw;
    height: 5vh;
    font-size: large;
  }
}

@media screen and (max-width: 480px) {
  .fullscreen {
    overflow-y: scroll;
    padding: 16px;
  }

  header {
    font-size: 0.75rem;
    line-height: 1rem;
    height: auto;
  }
}
    </style>
    <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"
    integrity="sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY="
    crossorigin=""/>
    <!-- Make sure you put this AFTER Leaflet's CSS -->
    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js" integrity="sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo=" crossorigin=""></script>
</head>

<body class="fullscreen">
    <header>
        <div class="time">
            <!-- <span id="SyncSec">0</span> SEC -->
            Copyright © 2024 Sky's Ace
        </div>
    </header>

    <main>
        <section class="map-container">
            <div id="map"></div>
        </section>
        
        <section class="sensor-data">
            <h2>BMP280</h2>
            <ul>
                <li>Pressure : <span id="BMP280Press">0</span></li>
            </ul>
            
            <h2>MCP9808</h2>
            <ul>
                <li>Temp : <span id="MCP9808Temp">0</span></li>
            </ul>
            
            <h2>GPS</h2>
            <ul>
                <li>Latitude : <span id="GPSLat">0</span></li>
                <li>Longitude : <span id="GPSLng">0</span></li>
                <li>Altitude : <span id="GPSAltitude">0</span></li>
            </ul>
            
            <h2>GY521</h2>
            <ul>
                <li>Acc X : <span id="GY521AccX">0</span></li>
                <li>Acc Y : <span id="GY521AccY">0</span></li>
                <li>Acc Z : <span id="GY521AccZ">0</span></li>
                <li>Gy X : <span id="GY521GyX">0</span></li>
                <li>Gy Y : <span id="GY521GyY">0</span></li>
                <li>Gy Z : <span id="GY521GyZ">0</span></li>
            </ul>
        </section>
    </main>

    <div class="phasephase">
        <h class="phase">Launch : <span id="Launch">0</span></h>
        <h class="phase">Deploy : <span id="Deploy">0</span></h>
        <h class="phase">Land : <span id="Land">0</span></h>
    </div>

    <sub>
        <section class="visual-data">
            <div> 
                <canvas id="temp-graph"></canvas>
            </div>
            <div>
                <div id="rocket-path"></div>
            </div>
            <div>
                <canvas id="press-graph"></canvas>
            </div>
        </section>
        <select name="choose-component" id="choose">
            <option value="temp-graph">temp-graph</option>
            <option value="press-graph">press-graph</option>
            <option value="rocket-path">rocket-path</option>
            <option value="gyro-visual">gyro-visualization</option>
        </select>
        <button class="start-btn" id="start-btn" type="button">Start</button>
        <button class="start-btn" id="download-btn" type="button" style="width: 10vw;">Download Log</button>
        <div class="cube-content">
            <div id="cube">
        </div>
    </sub>

    <!-- <footer>
        <div>
            
        </div>
    </footer> -->

    <script type="module" src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js"></script>
    <script type="module" src='https://cdnjs.cloudflare.com/ajax/libs/plotly.js/2.32.0/plotly.min.js'></script>
    <script type="module" src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
    <script type="module">
import { initializeApp } from "https://www.gstatic.com/firebasejs/10.11.1/firebase-app.js";
import { getDatabase, ref, child, get } from 'https://www.gstatic.com/firebasejs/10.11.1/firebase-database.js';

const firebaseConfig = {
  apiKey: "AIzaSyAqoFAWt3NbaND-BM7RzhXCOv5AFD19S4k",
  authDomain: "cansat24-dfef7.firebaseapp.com",
  databaseURL: "https://cansat24-dfef7-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "cansat24-dfef7",
  storageBucket: "cansat24-dfef7.appspot.com",
  messagingSenderId: "810679267387",
  appId: "1:810679267387:web:3335e5abb33a716a4c2c1e"
};

const app = initializeApp(firebaseConfig);
const db = getDatabase();
const dbRef = ref(db);

const sensorElements = {
  valueSyncSec: document.getElementById("SyncSec"),
  valuePress: document.getElementById("BMP280Press"),
  valueMCPTemp: document.getElementById("MCP9808Temp"),
  valueLat: document.getElementById("GPSLat"),
  valueLng: document.getElementById("GPSLng"),
  valueGPSAltitude: document.getElementById("GPSAltitude"),
  valueAccX: document.getElementById("GY521AccX"),
  valueAccY: document.getElementById("GY521AccY"),
  valueAccZ: document.getElementById("GY521AccZ"),
  valueGyX: document.getElementById("GY521GyX"),
  valueGyY: document.getElementById("GY521GyY"),
  valueGyZ: document.getElementById("GY521GyZ"),
};

let mapInitialized = false;
let cansatStatus = 0;
let updateStatusPreviousAltitude = 0;

function initMap(lat, lng) {
  if (mapInitialized) return;
  mapInitialized = true;
  const map = L.map('map', { center: [lat, lng], zoom: 18 });
  L.tileLayer('http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
    maxZoom: 22,
    attribution: "Skys Ace",
    subdomains: ['mt0', 'mt1', 'mt2', 'mt3']
  }).addTo(map);
}

function updateMap(sensorData) {
  if (!mapInitialized) return;
  const { valueLat: lat, valueLng: lng } = sensorData;
  L.marker([lat, lng]).addTo(map).panTo([lat, lng]);
}

function updateSensorData(sensorData) {
  Object.entries(sensorElements).forEach(([key, element]) => {
    element.innerHTML = sensorData[key];
  });
  updateStatus(sensorData);
}

function updateStatus(sensorData) {
  const altitude = sensorData.valueGPSAltitude;
  cansatStatus = altitude > updateStatusPreviousAltitude ? 1 : (altitude < updateStatusPreviousAltitude ? 2 : 3);
  updateStatusPreviousAltitude = altitude;

  const statusElementId = cansatStatus === 1 ? "Launch" : (cansatStatus === 2 ? "Deploy" : "Land");
  document.getElementById(statusElementId).innerHTML = "1";
}

function fetchData() {
  get(child(dbRef, `/`)).then((snapshot) => {
    const data = snapshot.val();
    if (data) {
      const formattedData = Object.values(data).map(sensorData => ({
        timestamp: sensorData.timestamp,
        valueMCPTemp: sensorData.valueMCPTemp,
        valuePress: sensorData.valuePress,
        valueLat: sensorData.valueLat,
        valueLng: sensorData.valueLng,
        valueGPSAltitude: sensorData.valueGPSAltitude,
      }));

      const selectedGraph = document.getElementById("choose").value;
      selectedGraph === "rocket-path" ? ThreeDChart(formattedData) : createChart(formattedData, selectedGraph);
    } else {
      console.warn("No data found in Firebase");
    }
  });
}

function createChart(data, graph) {
  const ctx = document.getElementById(graph).getContext('2d');
  const labels = data.map(point => `ID: ${point.id}`);

  const datasets = {
    "temp-graph": [{
      label: 'Temperature',
      data: data.map(point => ({ x: point.timestamp, y: point.valueMCPTemp })),
      backgroundColor: 'rgb(255, 0, 0)',
      borderColor: 'rgb(187, 0, 0)'
    }],
    "press-graph": [{
      label: 'Pressure',
      data: data.map(point => ({ x: point.timestamp, y: point.valuePress })),
      backgroundColor: 'rgb(120, 120, 120)',
      borderColor: 'rgb(60, 60, 60)'
    }]
  };

  new Chart(ctx, {
    type: 'scatter',
    data: { labels, datasets: datasets[graph] },
    options: {
      scales: {
        x: { title: { display: true, text: 'Time' } },
        y: { title: { display: true, text: graph === 'temp-graph' ? 'Temperature' : 'Pressure' } }
      }
    }
  });
}

function ThreeDChart(formattedData) {
  const xValues = formattedData.map(data => data.valueLat);
  const yValues = formattedData.map(data => data.valueLng);
  const zValues = formattedData.map(data => data.valueGPSAltitude);

  const datasets = [{
    x: xValues,
    y: yValues,
    z: zValues,
    mode: 'markers',
    type: 'scatter3d',
    marker: {
      color: 'rgb(10, 100, 240)',
      size: 2
    }
  }];

  const layout = {
    autosize: true,
    height: 300,
    scene: { aspectratio: { x: 1, y: 1, z: 1 } },
    margin: { l: 0, r: 0, b: 0, t: 0 }
  };

  Plotly.newPlot('rocket-path', datasets, layout);
}

let scene, camera, cube, renderer;

function initThreeD() {
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0xffffff);

  const cubeContainer = document.getElementById("cube");
  const cubeWidth = cubeContainer.parentElement.clientWidth;
  const cubeHeight = cubeContainer.parentElement.clientHeight;

  camera = new THREE.PerspectiveCamera(75, cubeWidth / cubeHeight, 0.1, 1000);
  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(cubeWidth, cubeHeight);
  cubeContainer.appendChild(renderer.domElement);

  const cubeMaterials = [
    new THREE.MeshBasicMaterial({ color: 0x03045e }),
    new THREE.MeshBasicMaterial({ color: 0x023e8a }),
    new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
    new THREE.MeshBasicMaterial({ color: 0x03045e }),
    new THREE.MeshBasicMaterial({ color: 0x023e8a }),
    new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
  ];

  const geometry = new THREE.BoxGeometry(5, 1, 4);
  const material = new THREE.MeshFaceMaterial(cubeMaterials);

  cube = new THREE.Mesh(geometry, material);
  scene.add(cube);
  camera.position.z = 5;
  renderer.render(scene, camera);
}

function animate(sensorData) {
  cube.rotation.x = sensorData.valueGyX;
  cube.rotation.y = sensorData.valueGyY;
  cube.rotation.z = sensorData.valueGyZ;
  renderer.render(scene, camera);
}

let log = [];

function download() {
  if (!log.length) {
    console.warn("No log data to download.");
    return;
  }

  const blob = new Blob([log.join('\n')], { type: 'text/plain' });
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = 'websocket_data.txt';
  a.click();
  URL.revokeObjectURL(url);
}

const ws = new WebSocket('ws://' + window.location.hostname + ':81/');

ws.onmessage = function (event) {
  const sensorData = JSON.parse(event.data);
  updateSensorData(sensorData);
  log.push(JSON.stringify(sensorData));

  if (!mapInitialized) {
    initMap(sensorData.valueLat, sensorData.valueLng);
  }

  updateMap(sensorData);
  animate(sensorData);
};

ws.onerror = function (error) {
  console.error("WebSocket error:", error);
};

document.getElementById("start-btn").onclick = function () {
  document.getElementById("choose").value === "gyro-visual" ? initThreeD() : fetchData();
};

document.getElementById("download-btn").onclick = function () { download() };

    </script>
</body>
</html>
)=====";
// ------------------------ //

// network //
const char* ssid = "CAT yamalah wifi";
const char* pass = "pppb1346";
unsigned long previousMillis = 0;
unsigned long interval = 5000;
// ------------------------ //

// webserver port //
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
// ------------------------ //

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
String BMPPressure = "/valuePress";
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
String timestamp = "/timestamp";
// ------------------------ //

String JSONtxt;
String incoming = "";
String valueSyncSec;
String valuePress;
String valueAccX;
String valueAccY;
String valueAccZ;
String valueGyX;
String valueGyY;
String valueGyZ;
String valueMCPTemp;
String valueLat;
String valueLng;
String valueGPSAltitude;

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

void handleRoot() {
  server.send(200, "text/html", mainpage);
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

  // init LoRa //
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa Error");
    while (1);
  }

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
  int packetSize = LoRa.parsePacket();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
  }
 
  if (LoRa.available()) {
    incoming += LoRa.readString();

    // case if sensor data is csv format //
    // BMP280 == temp, pressure, altitude
    // GY521 == Acceleration X Y Z and Gyro X Y Z
    // gps == lat, lng, altitude
    // mcp == temp
    String Module = getValue(incoming, ',', 0);

    if (Module == "BMP280") {
      valueSyncSec = getValue(incoming, ',', 1);
      valuePress = getValue(incoming, ',', 2);

      JSONtxt = "{\"valueSyncSec\":\"" + valueSyncSec + "\",";
      JSONtxt += "\"valuePress\":\"" + valuePress + "\"}";

      webSocket.broadcastTXT(JSONtxt);
    }
    else if (Module == "GY521") {
      valueSyncSec = getValue(incoming, ',', 1);
      valueAccX = getValue(incoming, ',', 2);
      valueAccY = getValue(incoming, ',', 3);
      valueAccZ = getValue(incoming, ',', 4);
      valueGyX = getValue(incoming, ',', 5);
      valueGyY = getValue(incoming, ',', 6);
      valueGyZ = getValue(incoming, ',', 7);

      JSONtxt = "{\"valueSyncSec\":\"" + valueSyncSec + "\",";
      JSONtxt += "\"valueAccX\":\"" + valueAccX + "\",";
      JSONtxt += "\"valueAccY\":\"" + valueAccY + "\",";
      JSONtxt += "\"valueAccZ\":\"" + valueAccZ + "\",";
      JSONtxt += "\"valueGyX\":\"" + valueGyX + "\",";
      JSONtxt += "\"valueGyY\":\"" + valueGyY + "\",";
      JSONtxt += "\"valueGyZ\":\"" + valueGyZ + "\"}";

      webSocket.broadcastTXT(JSONtxt);
    }
    else if (Module == "MCP9808") {
      valueSyncSec = getValue(incoming, ',', 1);
      valueMCPTemp = getValue(incoming, ',', 2);
      
      JSONtxt = "{\"valueSyncSec\":\"" + valueSyncSec + "\",";
      JSONtxt += "\"valueMCPTemp\":\"" + valueMCPTemp + "\"}";

      webSocket.broadcastTXT(JSONtxt);
    }
    else if (Module == "GPS") {
      valueSyncSec = getValue(incoming, ',', 1);
      valueLat = getValue(incoming, ',', 2);
      valueLng = getValue(incoming, ',', 3);
      valueGPSAltitude = getValue(incoming, ',', 4);

      JSONtxt = "{\"valueSyncSec\":\"" + valueSyncSec + "\",";
      JSONtxt += "\"valueLat\":\"" + valueLat + "\",";
      JSONtxt += "\"valueLng\":\"" + valueLng + "\",";
      JSONtxt += "\"valueGPSAltitude\":\"" + valueGPSAltitude + "\"}";

      webSocket.broadcastTXT(JSONtxt);
    }
    
    // firebase //
    if (Firebase.ready() && signupOK) {
      if (Module == "BMP280" || Module == "GY521" || Module == "MCP9808" || Module == "GPS") {
        if (currentMillis - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0) { // set store freq
          sendDataPrevMillis = currentMillis;
          databasePath = getValue(incoming, ',', 1) + "/";
          json.clear();
          if (Module == "BMP280") {
            json.set(BMPPressure.c_str(), valuePress);
          }
          else if (Module == "GY521") {
            json.set(GYAccX.c_str(), valueAccX);
            json.set(GYAccY.c_str(), valueAccY);
            json.set(GYAccZ.c_str(), valueAccZ);
            json.set(GYX.c_str(), valueGyX);
            json.set(GYY.c_str(), valueGyY);
            json.set(GYZ.c_str(), valueGyZ);
          }
          else if (Module == "MCP9808") {
            json.set(MCPTemp.c_str(), valueMCPTemp);
          }
          else if (Module == "GPS") {
            json.set(GPSLat.c_str(), valueLat);
            json.set(GPSLng.c_str(), valueLng);
            json.set(GPSAltitude.c_str(), valueGPSAltitude);
          }
          json.set(timestamp.c_str(), getValue(incoming, ',', 1));
          Serial.printf(" - Set json... %s - \n", Firebase.RTDB.setJSON(&fbdo, databasePath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
        }
      }
    }
    
    incoming = "";
  }
  server.handleClient();
}