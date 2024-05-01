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
    updateSenserData.previousAltitude = JSobj.valueGPSAltitude;
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