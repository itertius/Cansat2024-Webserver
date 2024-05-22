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

header {
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
  grid-template-columns: 50% 50%;
  padding: 1vh 0vw 3vh 0vw;
}

.sensor-data {
  display: grid;
  grid-template-columns: 50% 50%;
}

footer {
  background-color: whitesmoke;
  display: flex;
  justify-content: center;
  align-items: center;
  /* position: relative; */
  height: 10%;
  font-size: x-large;
  margin-top: 3vh;
}

#map {
  height: 100%;
  width: 100%;
  background-color: white;
}

h2,
.footer {
  text-align: center;
}

#cansat_phase {
  color: white;
}

.start-btn {
  display: block;
  margin: 0 auto;
  width: 5vw;
  height: 5vh;
  font-size: large;
}

#choose {
  display: block;
  margin: 0 auto;
  width: 10vw;
  height: 5vh;
  font: large;
}

sub {
  height: 40%;
  padding: 1vh 0vw 1vh 0vw;
}

.visual-data {
  display: grid;
  grid-template-columns: 33% 33% 33%;
  column-gap: 4px;
  justify-content: center;
  text-align: center;
  /* width: 10vw; */
  color: blue;
  padding: 3vh 1vw 1vh 1vw;
}

#temp-graph {
  background-color: #ffffff;
  width: 100%;
  height: 40vh;
}

#press-graph {
  background-color: #ffffff;
  width: 100%;
  height: 40vh;
}

#gyro-anime {
  background-color: #ffffff;
  width: auto;
  height: 40vh;
}

#rocket-path {
  background-color: #ffffff;
  width: 100%;
  height: 98%;
}

.cube-content {
  padding-top: 2vh;
  height: 25%;
}

.phasephase {
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
  justify-content: center;
  text-align: center;
}

.phase {
  color: white;
  font-size: larger;
}

#Launch,
#Deploy,
#Land {
  color: white;
  font-size: larger;
}

@media screen and (max-width: 1200px) {
  .main {
    padding: 6px;
  }
}

@media screen and (max-width: 768px) {
  #map {
    height: 100%;
    width: 100%;
    background-color: white;
  }
  main {
    display: grid;
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
  .header {
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

import { getDatabase, ref, child, get } from 'https://www.gstatic.com/firebasejs/10.11.1/firebase-database.js'

const db = getDatabase();
const dbRef = ref(getDatabase());

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
  const map = L.map('map', {
    center: [lat, lng],
    zoom: 18
  });
  L.tileLayer('http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
    maxZoom: 22,
    attribution: "Skys Ace",
    subdomains: ['mt0', 'mt1', 'mt2', 'mt3']
  }).addTo(map);
}

function updateMap(sensorData) {
  if (!map) return;
  const lat = sensorData.valueLat;
  const lng = sensorData.valueLng;
  const latlng = [[lat, lng]];
  const marker = L.marker([lat, lng]).addTo(map);
  if (latlng.length > 1) {
    map.panTo([lat, lng]);
  }
}

function updateSensorData(sensorData) {
  Object.keys(sensorElements).forEach(key => {
    sensorElements[key].innerHTML = sensorData[key];
  });
  updateStatus(sensorData);
}

function updateStatus(sensorData) {
  const altitude = sensorData.valueGPSAltitude;
  cansatStatus = altitude > updateStatusPreviousAltitude ? 1 : (altitude < updateStatusPreviousAltitude ? 2 : 3);
  updateStatusPreviousAltitude = altitude;
  if (cansatStatus==1) {
    document.getElementById("Launch").innerHTML = "1";
  }
  else if (cansatStatus==2) {
    document.getElementById("Deploy").innerHTML = "1";
  }
  else if (cansatStatus==3) {
    document.getElementById("Land").innerHTML = "1";
  }
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
      if (selectedGraph === "rocket-path") {
        ThreeDChart(formattedData);
      }
      else {
        createChart(formattedData, selectedGraph);
      }
    } else {
      console.warn("No data found in Firebase");
    }
  });
}

function createChart(data, graph) {
  const ctx = document.getElementById(graph).getContext('2d');
  const labels = data.map(point => `ID: ${point.id}`);

  const datasets = {
    "temp-graph": [
      {
        label: 'Temperature', data: data.map(point => ({
          x: point.timestamp, y: point.valueMCPTemp
        })), backgroundColor: 'rgb(255, 0, 0)', borderColor: 'rgb(187, 0, 0)'
      },
    ],
    "press-graph": [
      { label: 'Pressure', data: data.map(point => ({ x: point.timestamp, y: point.valuePress })), backgroundColor: 'rgb(120, 120, 120)', borderColor: 'rgb(60, 60, 60)' }
    ]
  };

  new Chart(ctx, {
    type: 'scatter',
    data: {
      labels,
      datasets: datasets[graph]
    },
    options: {
      scales: {
        x: { title: 'Time' },
        y: { title: graph === 'temp-graph' ? 'Temperature' : 'Pressure' }
      }
    }
  });
}

function ThreeDChart(formattedData) {
  const xValues = formattedData.map(data => data.valueLat);
  const yValues = formattedData.map(data => data.valueLng);
  const zValues = formattedData.map(data => data.valueGPSAltitude);

  var datasets = [{
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
  var layout = {
    autosize: true,
    height: 300,
    scene: {
      aspectratio: {
        x: 1,
        y: 1,
        z: 1
      }
    },
    margin: {
      l: 0,
      r: 0,
      b: 0,
      t: 0
    }
  };
  Plotly.newPlot('rocket-path', datasets, layout);
}
let scene, camera, cube, geometry, renderer, material, cubeWidth, cubeHeight;
function parentWidth(elem) {
  return elem.parentElement.clientWidth;
}
function parentHeight(elem) {
  return elem.parentElement.clientHeight;
}
function initThreeD() {
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0xffffff);
  cubeWidth = parentWidth(document.getElementById("cube"));
  cubeHeight = parentHeight(document.getElementById("cube"));
  camera = new THREE.PerspectiveCamera(75, cubeWidth / cubeHeight, 0.1, 1000);
  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(cubeWidth, cubeHeight);
  document.getElementById('cube').appendChild(renderer.domElement);

  var cubeMaterials = [
    new THREE.MeshBasicMaterial({ color: 0x03045e }),
    new THREE.MeshBasicMaterial({ color: 0x023e8a }),
    new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
    new THREE.MeshBasicMaterial({ color: 0x03045e }),
    new THREE.MeshBasicMaterial({ color: 0x023e8a }),
    new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
  ];

  geometry = new THREE.BoxGeometry(5, 1, 4);
  material = new THREE.MeshFaceMaterial(cubeMaterials);

  cube = new THREE.Mesh(geometry, material);
  scene.add(cube);
  camera.position.z = 5;
  renderer.render(scene, camera);
}

function animate(sensorData) {
  cube.rotation.x = sensorData.valueGyX;
  cube.rotation.y = sensorData.valueGyX;
  cube.rotation.z = sensorData.valueGyX;
  renderer.render(scene, camera);
}

var log=[];

function download() {
  if (log.length === 0) {
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
    mapInitialized = true;
  }
  updateMap(sensorData);
  animate(sensorData);
};

ws.onerror = function (error) {
  console.error("WebSocket error:", error);
};

document.getElementById("start-btn").onclick = function () { 
  if (document.getElementById("choose").value === "gyro-visual") {
    initThreeD();
  }
  else {
    fetchData();
  }
 };

document.getElementById("download-btn").onclick = function () { download() };
    </script>
</body>
</html>
)=====";