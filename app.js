
// Sync Sec //
var SyncSec = document.getElementById("SyncSec");
// -------------------------------- //

// BMP280 Component //
var BMP280Temp = document.getElementById("BMP280Temp");
var BMP280Press = document.getElementById("BMP280Press");
var BMP280Altitude = document.getElementById("BMP280Altitude");
// -------------------------------- //

// MCP9808 Component //
var MCP9808Temp = document.getElementById("MCP9808Temp");
// -------------------------------- //

// GPS Component //
var GPSLat = document.getElementById("GPSLat");
var GPSlng = document.getElementById("GPSLng");
var GPSAltitude = document.getElementById("GPSAltitude");
// -------------------------------- //

// GY521 Component //
var GY521AccX = document.getElementById("GY521AccX");
var GY521AccY = document.getElementById("GY521AccY");
var GY521AccZ = document.getElementById("GY521AccZ");
var GY521GyX = document.getElementById("GY521GyX");
var GY521GyY = document.getElementById("GY521GyY");
var GY521GyZ = document.getElementById("GY521GyZ");
// -------------------------------- //

// Phase Component //

// -------------------------------- //

// Marker //
var marker, path;
function addmarker() {
  latlng.push([JSobj.GPSLat, JSobj.GPSlng]);
  
  var lat = latlng[latlng.length-1][0]
  var lng = latlng[latlng.length-1][0]
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
function initmap() {
  map = L.map('map'),{
    center:(
      [14.542925388875888, 
      100.91013472649756], 
      18)}; // change LAT LONG HERE //
  L.tileLayer('http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
      maxZoom: 22,
      attribution: "Skys Ace",
      subdomains:['mt0','mt1','mt2','mt3']
  }).addTo(map);
}
// -------------------------------- //
InitWebSocket();

// WebSocket //
function InitWebSocket() {
  var ws = new WebSocket('') // ws URL

  ws.onmessage=function(event) {
    JSobj = JSON.parse(event.data);
    if(checkmap<1) {
      initmap();
      checkmap++;
    }
    BMP280Temp.innerHTML = JSobj.valueTemp;
    BMP280Press.innerHTML = JSobj.valuePress;
    BMP280Altitude.innerHTML = JSobj.valueBMPAltitude;

    GY521AccX.innerHTML = JSobj.valueAccX;
    GY521AccY.innerHTML = JSobj.valueAccY;
    GY521AccZ.innerHTML = JSobj.valueAccZ;
    GY521GyX.innerHTML = JSobj.valueGyX;
    GY521GyY.innerHTML = JSobj.valueGyY;
    GY521GyZ.innerHTML = JSobj.valueGyZ;

    GPSLat.innerHTML = JSobj.valueLat;
    GPSlng.innerHTML = JSobj.valueLng;
    GPSAltitude.innerHTML = JSobj.valueGPSAltitude;

    SyncSec.innerHTML = JSobj.valueSyncSec;
  }
  addmarker();
}

// -------------------------------- //
