## 🎯 Le Cinq Parfait
  
**Vidéo de présentation :** [Lien vers la vidéo (Linkedin)](https://www.linkedin.com/posts/j%C3%A9r%C3%A9my-dezetree_arduino-javascript-html-activity-7379918776978550784-QAi8?utm_source=share&utm_medium=member_desktop&rcm=ACoAADi1vzoBPj26Uphv3Rmwcbmo4awnomE7ono)
  
Un jeu de réflexes ludique mêlant électronique (Arduino) et développement web (JavaScript/Node.js).

Le principe est simple : un compteur de 10 secondes s'écoule en temps réel sur une interface web et sur un afficheur physique. Votre objectif ? Appuyer sur le bouton physique avec une précision à la milliseconde près pour stopper le chronomètre exactement sur 5.00. La pression du doigt est communiquée en temps réel au serveur pour valider ou non la tentative.

## 🛠️ Architecture du Projet
Ce projet repose sur une communication bidirectionnelle entre du matériel physique et une interface web via un serveur Node.js.

**Matériel (Arduino - buzzer.ino) :** Gère l'affichage matériel sur un afficheur 7 segments (4 digits), la logique du chronomètre de 10 secondes et la détection anti-rebond (debounce) du bouton poussoir.
**Serveur (Node.js - server.js) :** Fait le pont entre l'Arduino et l'interface web. Il lit les données matérielles via le port série (SerialPort) et les diffuse en temps réel aux clients connectés via des WebSockets. C'est lui qui valide la condition de victoire (arrêt à 5.00).
**Frontend Web (index.html & styles.css) :** Une Interface Homme-Machine (IHM) synchronisée en temps réel avec l'Arduino, affichant le chronomètre et l'état de la partie (En attente, En cours, Victoire, Échec).

## 🧰 Matériel Requis

- 1x Carte Arduino (Uno, Nano, Mega, etc.)
- 1x Afficheur 7 segments à 4 chiffres
- 1x Bouton poussoir
- Câbles de connexion et Breadboard

## 🚀 Installation et Démarrage

### 1. Configuration de l'Arduino

Ouvrez le fichier buzzer.ino dans l'Arduino IDE.
__Câblez votre Arduino selon les pins définies dans le code :__
- Bouton : Pin 13
- Afficheur (Segments a-g, p) : Pins 1 à 8
- Afficheur (Digits d1-d4) : Pins 9 à 12
- Téléversez le code sur votre carte Arduino.

Important : Fermez le moniteur série de l'IDE Arduino pour libérer le port série.

### 2. Configuration du Serveur

Assurez-vous d'avoir Node.js installé sur votre machine.
Ouvrez un terminal dans le dossier racine du projet.
Initialisez le projet et installez les dépendances requises :

```bash
npm init -y
npm install express ws serialport @serialport/parser-readline
```

Note : Si votre Arduino n'est pas sur le port /dev/ttyACM0 (ex: COM3 sur Windows), pensez à modifier la constante ARDUINO_PORT au début du fichier server.js.

Lancez le serveur :
```bash
node server.js
```

###  3. Jouer

Ouvrez votre navigateur web et rendez-vous sur : http://localhost:3000
Appuyez sur le bouton physique de votre Arduino pour lancer le chronomètre.
Appuyez à nouveau pour tenter de l'arrêter sur 5.00 tout pile !
Appuyez une troisième fois pour réinitialiser le jeu.

## 🎮 États du Jeu
🔵 WAITING : Le jeu est prêt. Appuyez sur le bouton pour commencer.

🟡 RUNNING : Le chrono défile, préparez-vous.

🟢 STOPPED_WIN : Bravo ! Vous avez arrêté le compteur à 5.00.

🔴 STOPPED_FAIL : Vous avez arrêté le compteur trop tôt, trop tard, ou vous l'avez laissé tomber à 0.00.

Développé avec précision (et beaucoup de gestion de millisecondes) !