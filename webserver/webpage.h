const char webpage[] PROGMEM = R"=====(
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
  height: 98vh;
  overflow: hidden;
  background-color: black;
}

.header {
  display: flex;
  justify-content: center;
  align-items: center;
  font-size: x-large;
  height: 10%;
  background-color: whitesmoke;
}

.main {
  background-color: rgb(12, 12, 21);
  height: 80%;
  color: white;
  display: grid;
  grid-template-columns: 50% 50%;
  padding: 4px;
}

.sensor-data {
  display: grid;
  grid-template-columns: 50% 50%;
}

.footer {
  background-color: whitesmoke;
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  justify-content: center;
  align-items: center;
  height: 10%;
  font-size: x-large;
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

@media screen and (max-width: 1200px) {
  .main {
    padding: 6px;
  }
}

@media screen and (max-width: 768px) {
  .main {
    display: flex;
    flex-direction: column;
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
    <header class="header">
        <div class="time">
            <span id="SyncSec">0</span>
        </div>
    </header>

    <main class="main">
        <section class="map-container">
            <div id="map"></div>
        </section>
        
        <section class="sensor-data">
            <h2>BMP280</h2>
            <ul>
                <li>Temp : <span id="BMP280Temp">0</span></li>
                <li>Pressure : <span id="BMP280Press">0</span></li>
                <li>ALtitude : <span id="BMP280Altitude">0</span></li>
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

    <footer class="footer">
        <div class="footer-left" id="cansat_phase">
            cansat_status
        </div>
        <div class="footer-right">
            Copyright © 2024 Sky's Ace
        </div>
    </footer>

    <script>
var latlng = [];
var marker;
var map;
var checkmap = 0;
var cansat_status = 0;

var sensorElements = {
    valueSyncSec: document.getElementById("SyncSec"),
    valueTemp: document.getElementById("BMP280Temp"),
    valuePress: document.getElementById("BMP280Press"),
    valueBMPAltitude: document.getElementById("BMP280Altitude"),
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

function initmap(lat, lng) {
    map = L.map('map', {
        center: [lat, lng],
        zoom: 18
    });
    L.tileLayer('http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
        maxZoom: 22,
        attribution: "Skys Ace",
        subdomains: ['mt0', 'mt1', 'mt2', 'mt3']
    }).addTo(map);
}

function updateMap(JSobj) {
    if (!map) return;
    latlng.push([JSobj.valueLat, JSobj.valueLng]);
    var lat = latlng[latlng.length - 1][0];
    var lng = latlng[latlng.length - 1][1];
    marker = L.marker([lat, lng]).addTo(map);
    if (latlng.length > 1) {
        map.panTo([lat, lng]);
    }
}

function updateSenserData(JSobj) {
    Object.keys(sensorElements).forEach(key => {
        sensorElements[key].innerHTML = JSobj[`${key}`];
    });
    updateStatus(JSobj);
}

function updateStatus(JSobj) {
    const previousAltitude = updateSenserData.previousAltitude || JSobj.valueGPSAltitude;
    updateSenserData.previousAltitude = JSobj.ดvalueGPSAltitude;
    cansat_status = JSobj.valueGPSAltitude > previousAltitude ? 1 : JSobj.valueGPSAltitude < previousAltitude ? 2 : 3;
    console.log(cansat_status);

}

var ws = new WebSocket('ws://' + window.location.hostname + ':81/');

ws.onmessage = function (event) {
    const JSobj = JSON.parse(event.data);
    updateSenserData(JSobj);
    if (checkmap < 1) {
        initmap(JSobj.valueLat, JSobj.valueLng);
        checkmap++;
    }
    updateMap(JSobj);
};

ws.onerror = function (error) {
    console.error("WebSocket error:", error);
};
    </script>
</body>
</html>
)=====";
