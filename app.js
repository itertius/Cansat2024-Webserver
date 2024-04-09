

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
}


// Phase Component //

// -------------------------------- //

var latlng=[]
// Marker //
var marker, path;
function updateMap() {
  if (!map) return;
  latlng.push([JSobj.valueLat, JSobj.valueLng]);
  var lat = latlng[latlng.length-1][0]
  var lng = latlng[latlng.length-1][1]
  marker = L.marker([lat,lng]).addTo(map);
  path = L.polyline(latlng).addTo(map);
  if (latandlng.length>1){
    map.panTo([lat,lng]);
   }
}
// -------------------------------- //

// Map Component //
var map;
var checkmap=0;
function initmap(lat, lng) {
  map = L.map('map',{
    center:[lat, lng], zoom: 18});
  L.tileLayer('http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
      maxZoom: 22,
      attribution:"Skys Ace",
      subdomains:['mt0','mt1','mt2','mt3']
  }).addTo(map);
}
// -------------------------------- //

// WebSocket //
function InitWebSocket() {
  var ws = new WebSocket('ws://' + window.location.hostname + ':81/');

  ws.onmessage=function(event) {
    JSobj = JSON.parse(event.data);
    updateSenserData(JSobj)
    if(checkmap<1) {
      initmap(JSobj.valueLat, JSobj.valueLng);
      checkmap++;
    }
    updateMap();
  };
  
  ws.onerror = function(error) {
    console.error("WebSocket error:", error);
  };
}
// -------------------------------- //
InitWebSocket();