const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const WebSocket = require('ws');
const http = require('http');
const express = require('express');
const path = require('path');

const ARDUINO_PORT = '/dev/ttyACM0';
const BAUDRATE = 9600;
const WEB_PORT = 3000;

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

let gameState = 'WAITING';

app.use(express.static(path.join(__dirname, 'public')));
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

server.listen(WEB_PORT, () => {
    console.log(`Serveur web lancé sur http://localhost:${WEB_PORT}`);
    console.log(`WebSocket Server actif.`);
});

const port = new SerialPort({ path: ARDUINO_PORT, baudRate: BAUDRATE });
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

port.on('open', () => {
    console.log(`Connexion au port série ${ARDUINO_PORT} établie.`);
});

port.on('error', (err) => {
    console.error(`Erreur de port série: ${err.message}`);
    console.error(`Vérifiez que l'Arduino est branché sur ${ARDUINO_PORT} et que le moniteur série de l'IDE est fermé.`);
});

parser.on('data', data => {
    const line = data.trim();
    console.log(`> Arduino: ${line}`);

    let counterValue = null;

    if (line === "RUNNING") {
        gameState = 'RUNNING';
    } else if (line === "RESET") {
        gameState = 'WAITING';
        counterValue = "10.00";
    } else if (line === "FAILED") {
        gameState = 'STOPPED_FAIL';
        counterValue = "0.00";
    } else if (line.startsWith("STOP")) {
        gameState = 'STOPPED';
    } else if (!isNaN(parseFloat(line))) {
        counterValue = line;

        if (gameState === 'STOPPED') {
            if (counterValue === "5.00") {
                gameState = 'STOPPED_WIN';
                console.log("🎉 VICTOIRE ! Compteur stoppé à 5.00");
            } else if (parseFloat(counterValue) < 0.0) {
                gameState = 'STOPPED_FAIL';
            } else {
                gameState = 'STOPPED_FAIL';
                console.log(`😢 DÉFAITE ! Stoppé à ${counterValue}.`);
            }
        }
    }

    if (counterValue !== null) {
        const message = JSON.stringify({
            counter: counterValue,
            state: gameState
        });

        wss.clients.forEach(client => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(message);
            }
        });
    }
});

wss.on('connection', ws => {
    console.log('Nouveau client WebSocket connecté');

    const initialMessage = JSON.stringify({
        counter: "10.00",
        state: gameState
    });
    ws.send(initialMessage);

    ws.on('close', () => {
        console.log('Client WebSocket déconnecté');
    });
});