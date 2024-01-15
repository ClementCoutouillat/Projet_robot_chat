# Projet_robot_chat

Projet fait par Quentin Henriet, Bastien Chinianga, Jiang Kaixuan, Jangbo Wang et Clément Coutouillat
Dans ce projet, nous devons réaliser un robot à propulsion différentielle à deux roues pour jouer au jeu du chat et de la souris. Lorsque le robot est le chat, il poursuit la souris, et lorsqu'il est la souris, il fuit le chat. Pour accomplir ce jeu, nous devons réaliser les parties suivantes :

- Le robot doit pouvoir changer de rôle entre chat et souris dans le jeu. Lorsque deux robots entrent en collision, leurs rôles changent - capteur de détection de collision.
- Le robot doit pouvoir se déplacer dans un environnement avec des limites, donc nous avons besoin d'un capteur de détection de bord - capteur de détection de bord.
- Le robot doit pouvoir détecter le chat ou la souris sur le terrain, donc nous avons besoin d'un capteur pour détecter les objets sur le terrain - capteur LIDAR.

## Ydlidar

Nous utilisons le LIDAR YDLIDAR X4 pour détecter l'autre robot sur le terrain.
D'après le manuel de données de YDLIDAR, nous savons que YDLIDAR utilise Uart pour la communication. Dans le manuel de développement, nous pouvons obtenir le format des données du LIDAR, comme illustré ci-dessous :
Lorsque nous envoyons la commande A5 60 au LIDAR, il renvoie une trame de données, dont le format est illustré ci-dessous :
![image](https://github.com/ClementCoutouillat/Projet_robot_chat/blob/main/picture/scanCommand.png)

Ensuite, en suivant la méthode d'analyse du manuel de développement, nous obtenons les angles et les distances des obstacles de 0 à 360 degrés autour du robot, ce qui nous permet de localiser l'autre robot.
![image](https://github.com/ClementCoutouillat/Projet_robot_chat/blob/main/picture/receivedata.png)

## Stratégie

Dans le jeu, lorsque le robot est le chat, il doit poursuivre la souris, et lorsqu'il est la souris, il doit fuir le chat. Donc, selon le rôle du robot, nous avons besoin de stratégies différentes pour contrôler le mouvement du robot.

### Stratégie Chat

Lorsque le robot est le chat, il doit poursuivre la souris. Nous devons donc faire bouger le robot dans la direction de la souris. Tout d'abord, nous contrôlons la rotation du robot en calculant la différence d'angle entre le robot et la souris. Lorsque la souris se trouve entre 0 et 180 degrés, nous faisons tourner le robot à gauche. Lorsque l'autre robot se trouve entre 180 et 360 degrés, nous faisons tourner le robot

à droite. Une fois que le robot est orienté correctement, nous le faisons avancer jusqu'à ce qu'il entre en collision avec la souris. À ce moment-là, le rôle du robot change : le robot devient la souris et l'autre robot devient le chat. Nous pouvons alors utiliser la stratégie de la souris pour contrôler le mouvement du robot.

### Stratégie Souris

Lorsque le robot est la souris, il doit fuir le chat. Nous devons donc faire éloigner le robot du chat. Lorsqu'un chat se trouve à moins de 50 cm du robot et qu'il est situé entre 0 et 180 degrés, nous faisons tourner le robot à gauche de 90 degrés, puis le faisons avancer sur une certaine distance avant de le faire tourner à droite de 90 degrés. Cela permet au robot de s'éloigner du chat. Lorsque la distance entre le robot et le chat dépasse 50 cm, nous arrêtons le mouvement du robot. Lorsque le chat est situé entre 180 et 360 degrés, nous faisons tourner le robot à droite de 90 degrés, puis à gauche de 90 degrés, pour lui permettre de s'éloigner du chat.

## Moteur

Nous utilisons un robot à propulsion différentielle à deux roues, donc nous devons contrôler la vitesse des roues gauche et droite pour diriger le robot. Nous utilisons un moteur à courant continu à engrenages avec encodeur, dont la vitesse maximale est de 300 RPM.

### Avancer

Pour faire avancer le robot en ligne droite, nous contrôlons les vitesses des roues gauche et droite pour qu'elles soient identiques.

### Tourner

Le robot tourne en contrôlant la différence de vitesse entre les roues gauche et droite. Lorsque les vitesses des roues sont différentes, le robot tourne.

Nous utilisons une vitesse fixe pour contrôler la direction du robot. Lorsque le robot doit tourner à gauche, nous fixons la vitesse de la roue gauche à 0 et celle de la roue droite à la vitesse maximale. Lorsque le robot doit tourner à droite, nous fixons la vitesse de la roue droite à 0 et celle de la roue gauche à la vitesse maximale. Le temps nécessaire pour tourner est calculé avec la formule suivante :
![image](https://github.com/ClementCoutouillat/Projet_robot_chat/blob/main/picture/motor.jpg)

<br />
leftWheelLinearVelocity = wheelDiameter \* PI \* leftWheelRPM / 60 ;
<br />
rightWheelLinearVelocity = wheelDiameter \* PI \* rightWheelRPM / 60 ;
<br />
angulaVelocity = (leftWheellinearVelocity - rightWheellinearVelocity) / WheelDistance ;
<br />
Timer = Angle(0-360) \*PI / 180 / angulaVelocity ;

### Distance Control

La distance parcourue par le robot est contrôlée en gérant le temps de fonctionnement du robot. Lorsque le robot fonctionne pendant le temps que nous avons défini, il s'arrête. Cela permet de contrôler la distance parcourue par le robot. Le temps est calculé avec la formule suivante :
<br />
Timer = Distance / (WheelDiameter \* PI) / Vitesse ;

### Commande

Dans notre code pour manipuler le moteur, nous avons écrit deux fonctions. La première permet de contrôler directement la vitesse du moteur. Pour que le robot réponde plus rapidement aux commandes d'arrêt et d'accélération, nous avons également écrit une seconde fonction d'asservissement, qui utilise l'algorithme PID pour contrôler la rotation du moteur. Toutes les 100 ms, nous calculons la vitesse actuelle et fournissons un feedback sur la vitesse pour nous informer de la vitesse actuelle de rotation du moteur, ce qui nous permet de calculer plus précisément la position actuelle du robot et la distance parcourue. L'entrée du contrôleur PID est la vitesse cible du robot, et la sortie est la vitesse des roues gauche et droite du robot.

## Border

## Contact
