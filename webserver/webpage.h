const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sky's Ace</title>
    <style>
@import url("https://fonts.googleapis.com/css2?family=Poppins:wght@400;500;600;700;800;900&display=swap");

.fullscreen {
  width: 100%;
  height: 100vh;
  font-family: 'Poppins', sans-serif;
  font-weight: 600;
  overflow: hidden;
  background-color: whitesmoke;
}

.header {
  /* padding-left: 6px;
  padding-right: 6px; */
  display: flex;
  justify-content: center;
  align-items: center;
  text-align: center;
  font-size: x-large;
  height: 10%;
  background-color: whitesmoke;
}

.main {
  background-color: #00040f;
  /* padding-left: 6px;
  padding-right: 6px;
  padding-top: 4px;
  padding-bottom: 4px; */
  height: 80%;
  color: white;
  /* gap: 4px; */
  display: grid;
  grid-template-columns: 50% 50%;
  height: 80%;
}

.footer {
  height: 10%;
  display: flex;
  justify-content: center;
  align-items: center;
  font-size: x-large;
}

.right {
  display: grid;
  grid-template-columns: 50% 50%;
}

.leftleft {
  display: grid;
  grid-template-rows: 50% 50%;
}

.rightright {
  display: grid;
  grid-template-rows: 50% 50%;
}

#map {
  height: 100%;
  width: 98%;
  background-color: white;
}

.Phase {
  /* padding-top: 2px; */
}

/* xs : 480px */
@media screen and (max-width: 480px) {
  .fullscreen {
    overflow-y: scroll;
    padding-left: 16px;
    padding-right: 16px;
  }
  .header {
    font-size: 0.75rem;
    line-height: 1rem;
    height: auto;
  }
  .main {
    height: auto;
    display: flex;
    flex-direction: column;
  }
}

/* sm: 768px */
@media screen and (max-width: 768px) {
  .main {
    height: auto;
    display: flex;
    flex-direction: column;
  }
}

/* lg: 1200px */
@media screen and (max-width: 1200px) {
  .main {
    padding-left: 6px;
    padding-right: 6px;
    padding-top: 4px;
    padding-bottom: 4px;
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
        <div>
            
        </div>
        <div class="time">
            <div id="SyncSec">
                0
            </div>
        </div>
    </header>

    <div class="main">
        <div class="left">
            <div id="map"></div>
        </div>
        
        <div class="right">

            <div class="leftleft">
                <div class="BMP280" id="BMP280">
                    BMP280
                    <div id="BMP280Temp">
                        0
                    </div>
                    <div id="BMP280Press">
                        0
                    </div>
                    <div id="BMP280Altitude">
                        0
                    </div>
                </div>
                
                <div class="MCP9808" id="MCP9808">
                    MCP9808
                    <div id="MCP9808Temp">
                        0
                    </div>
                </div>
            </div>
            
            <div class="rightright">
                <div class="GPS" id="GPS">
                    GPS
                    <div id="GPSLat">
                        0
                    </div>
                    <div id="GPSLng">
                        0
                    </div>
                    <div id="GPSAltitude">
                        0
                    </div>
                </div>
                
                <div class="GY521" id="GY521">
                    GY521
                    <div id="GY521AccX">
                        0
                    </div>
                    <div id="GY521AccY">
                        0
                    </div>
                    <div id="GY521AccZ">
                        0
                    </div>
                    <div id="GY521GyX">
                        0
                    </div>
                    <div id="GY521GyY">
                        0
                    </div>
                    <div id="GY521GyZ">
                        0
                    </div>
                </div>
            </div>
        </div>
            
        
        <div class="Phase" id="Phase">
            Phase
        </div>
        
    </div>

    <footer class="footer">
        Copyright © 2024 Sky's Ace
    </footer>
</body>

<script>
import mysql from 'mysql2'
var latlng = [];
var marker, path;
var map;
var checkmap = 0;

function updateSenserData(JSobj) {
  // Sync Sec //
  var SyncSec = document.getElementById("SyncSec");
  SyncSec.innerHTML = JSobj.valueSyncSec;
  // -------------------------------- //

  // BMP280 Component //
  var BMP280Temp = document.getElementById("BMP280Temp");
  var BMP280Press = document.getElementById("BMP280Press");
  var BMP280Altitude = document.getElementById("BMP280Altitude");
  BMP280Temp.innerHTML = JSobj.valueTemp;
  BMP280Press.innerHTML = JSobj.valuePress;
  BMP280Altitude.innerHTML = JSobj.valueBMPAltitude;
  // -------------------------------- //

  // MCP9808 Component //
  var MCP9808Temp = document.getElementById("MCP9808Temp");
  MCP9808Temp.innerHTML = JSobj.valueMCPTemp;
  // -------------------------------- //

  // GPS Component //
  var GPSLat = document.getElementById("GPSLat");
  var GPSlng = document.getElementById("GPSLng");
  var GPSAltitude = document.getElementById("GPSAltitude");
  GPSLat.innerHTML = JSobj.valueLat;
  GPSlng.innerHTML = JSobj.valueLng;
  GPSAltitude.innerHTML = JSobj.valueGPSAltitude;
  // -------------------------------- //

  // GY521 Component //
  var GY521AccX = document.getElementById("GY521AccX");
  var GY521AccY = document.getElementById("GY521AccY");
  var GY521AccZ = document.getElementById("GY521AccZ");
  GY521AccX.innerHTML = JSobj.valueAccX ;
  GY521AccY.innerHTML = JSobj.valueAccY;
  GY521AccZ.innerHTML = JSobj.valueAccZ;
  var GY521GyX = document.getElementById("GY521GyX");
  var GY521GyY = document.getElementById("GY521GyY");
  var GY521GyZ = document.getElementById("GY521GyZ");
  GY521GyX.innerHTML = JSobj.valueGyX;
  GY521GyY.innerHTML = JSobj.valueGyY;
  GY521GyZ.innerHTML = JSobj.valueGyZ;
  // -------------------------------- //

  // prepare data for MySql //
  const data = {
    valueSyncSec: JSobj.valueSyncSec,
    valueTemp: JSobj.valueTemp,
    valuePress: JSobj.valuePress,
    valueBMPAltitude: JSobj.valueBMPAltitude,
    valueMCPTemp: JSobj.valueMCPTemp,
    valueLat: JSobj.valueLat,
    valueLng: JSobj.valueLng,
    valueGPSAltitude: JSobj.valueGPSAltitude,
    valueAccX: JSobj.valueAccX,
    valueAccY: JSobj.valueAccY,
    valueAccZ: JSobj.valueAccZ,
    valueGyX: JSobj.valueGyX,
    valueGyY: JSobj.valueGyY,
    valueGyZ: JSobj.valueGyZ,
  // -------------------------------- //
  }
  // call updateDatabase function //
  updateDatabase(data);
}

// Phase Component //

// -------------------------------- //

// Marker //
function updateMap(JSobj) {
  if (!map) return;
  latlng.push([JSobj.valueLat, JSobj.valueLng]);
  var lat = latlng[latlng.length-1][0]
  var lng = latlng[latlng.length-1][1]
  marker = L.marker([lat,lng]).addTo(map);
  path = L.polyline(latlng).addTo(map);
  if (latlng.length>1){
    map.panTo([lat,lng]);
   }
}
for (let i=0; i<500; i++) {
  updateMap(JSobj);
}
// -------------------------------- //

// Map Component //
function initmap(lat, lng) {
  map = L.map('map',{
    center: [lat, lng], zoom: 18});
  L.tileLayer('http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
      maxZoom: 22,
      attribution:"Skys Ace",
      subdomains:['mt0','mt1','mt2','mt3']
  }).addTo(map);
}
// -------------------------------- //

// update database function //
function updateDatabase(data) {
  // create pool //
  const pool = mysql.createPool({
    host: '127.0.0.1', // localhost
    user: 'root',
    password: 'iterrius',
    database: 'skybase', // change to actual database
  }).promise();

  // define sql query //
  const sql = `
    UPDATE sensor_data
    SET valueSyncSec = ?, valueTemp = ?, valuePress = ?, valueBMPAltitude = ?,
        valueMCPTemp = ?, valueLat = ?, valueLng = ?, valueGPSAltitude = ?,
        valueAccX = ?, valueAccY = ?, valueAccZ = ?, valueGyX = ?, valueGyY = ?, valueGyZ = ?
    WHERE id = ?
  `;

  // .exe update //
  pool.query(sql, Object.values(data))
    .then((results) => {
      console.log('Sensor data updated:', results);
    })
    .catch((error) => {
      console.error('Error updating sensor data:', error);
    })
    .finally(() => {
      pool.end();
    });
}

// WebSocket //
function InitWebSocket() {
  var ws = new WebSocket('ws://' + window.location.hostname + ':81/');

  console.log("init");

  ws.onmessage=function(event) {
    JSobj = JSON.parse(event.data);
    updateSenserData(JSobj)
    if(checkmap<1) {
      initmap(JSobj.valueLat, JSobj.valueLng); // num for test // when deloy change to initmap(JSobj.valueLat, JSobj.valueLng);
      checkmap++;
    }
    updateMap(JSobj);
  };
  
  ws.onerror = function(error) {
    console.error("WebSocket error:", error);
  };
}
// -------------------------------- //
InitWebSocket();
</script>
</html>
)=====";
