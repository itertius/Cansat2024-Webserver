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
}

function fetchData() {
  get(child(dbRef, `/`)).then((snapshot) => {
    const data = snapshot.val();
    if (data) {
      const formattedData = Object.values(data).map(sensorData => ({
        timestamp: sensorData.timestamp,
        valueTemp: sensorData.valueTemp,
        valueMCPTemp: sensorData.valueMCPTemp,
        valuePress: sensorData.valuePress,
        valueLat: sensorData.valueLat,
        valueLng: sensorData.valueLng,
        valueGPSAltitude: sensorData.valueGPSAltitude,
      }));

      const selectedGraph = document.getElementById("choose").value;
      if (selectedGraph === "rocket-path") {
        ThreeDChart(formattedData);
      } else {
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
        label: 'MCP9808 Temperature', data: data.map(point => ({
          x: point.timestamp, y: point.valueMCPTemp
        })), backgroundColor: 'rgb(255, 0, 0)', borderColor: 'rgb(187, 0, 0)'
      },
      { label: 'BMP280 Temperature', data: data.map(point => ({ x: point.timestamp, y: point.valueTemp })), backgroundColor: 'rgb(106, 90, 205)', borderColor: 'rgb(0, 0, 255)' }
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
    height: 500,
    scene: {
      aspectratio: {
        x: 1,
        y: 1,
        z: 1
      }
    }
  };
  Plotly.newPlot('rocket-path', datasets, layout);
}

const ws = new WebSocket('ws://' + window.location.hostname + ':81/');
ws.onmessage = function (event) {
  const sensorData = JSON.parse(event.data);
  updateSensorData(sensorData);
  if (!mapInitialized) {
    initMap(sensorData.valueLat, sensorData.valueLng);
    mapInitialized = true;
  }
  updateMap(sensorData);
};

ws.onerror = function (error) {
  console.error("WebSocket error:", error);
};

document.getElementById("start-btn").onclick = function () { fetchData() };