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

let mapInitialized = false;
let cansatStatus = 0;

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

function updateMap(JSobj) {
  if (!map) return;
  const lat = JSobj.valueLat;
  const lng = JSobj.valueLng;
  const latlng = [[lat, lng]];
  const marker = L.marker([lat, lng]).addTo(map);
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
  const altitude = JSobj.valueGPSAltitude;
  cansatStatus = altitude > (updateStatus.previousAltitude || altitude)? 1 : altitude < (updateStatus.previousAltitude || altitude)? 2 : 3;
  updateStatus.previousAltitude = altitude;
  console.log(cansatStatus);
}

function fetchData() {
  get(child(dbRef, `/`)).then((snapshot) => {
    const { data } = snapshot.val();
    if (data) {
      const formattedData = Object.values(data).map(sensorData => ({
        timestamp: sensorData.timestamp,
        valueTemp: sensorData.valueTemp,
        valueMCPTemp: sensorData.valueMCPTemp,
      }));
      createChart(formattedData);
    } else {
      console.warn("No data found in Firebase");
    }
  });
}
document.getElementsByClassName("start-btn")[0].onclick = fetchData;

function createChart(data) {
  const ctx = document.getElementById('myChart').getContext('2d');
  const labels = data.map(point => `ID: ${point.id}`);
  const datasets = [
    {
      label: 'MCPTemp',
      data: data.map(point => ({ x: point.timestamp, y: point.valueMCPTemp })),
      backgroundColor: 'rgb(255, 0, 0)',
      borderColor: 'rgb(187, 0, 0)',
    },
    {
      label: 'Temp',
      data: data.map(point => ({ x: point.timestamp, y: point.valueTemp })),
      backgroundColor: 'rgb(106, 90, 205)',
      borderColor: 'rgb(0, 0, 255)',
    }];

  new Chart(ctx, {
    type: 'scatter',
    data: {
      labels,
      datasets,
    },
    options: {
      scales: {
        x: {
          title: 'Time',
        },
        y: {
          title: 'Temperature',
        }
      }
    }
  });
}

const ws = new WebSocket('ws://' + window.location.hostname + ':81/');
ws.onmessage = function (event) {
  const JSobj = JSON.parse(event.data);
  updateSenserData(JSobj);
  if (!mapInitialized) {
    initMap(JSobj.valueLat, JSobj.valueLng);
    mapInitialized = true;
  }
  updateMap(JSobj);
};

ws.onerror = function (error) {
  console.error("WebSocket error:", error);
};1