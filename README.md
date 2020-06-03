# Dancing_Apart

Dancing_Apart is a project of IaaC, Institute for Advanced Architecture of Catalonia, developed at Master in Robotics and Advanced Construction in 2019-2020
Students: Cedric Droogmans, Luis Jayme Buerba, Elena Jaramazovic and Lorenzo Masini
Faculty: Angel Muñoz and Agustina Palazzo


# Concept

Using a polarplotter to trace the hand movement of a dancer that is in a different location then then the polarplotter.
![alt text](https://github.com/cedricdroogmans/Dancing_Apart/blob/master/images/concept.jpg)


# Software

Everything that is happening online is programed in the p5.js webeditor.
Using Node.js to setup a server socket.io to create the websockets to comunictate between the different clients, and ngrok to tunnel the port of the server. To comunicate between the arduino and the DrawBot side we are using p5.serialcontrol.

![alt text](https://github.com/cedricdroogmans/Dancing_Apart/blob/master/images/SWD_01.jpg)

Dancers side:
https://editor.p5js.org/Cedric.Droogmans/sketches/syienGx0-

DrawBot side:
https://editor.p5js.org/Cedric.Droogmans/sketches/MUZqmLO2C

Arduino code:
![alt text](https://github.com/cedricdroogmans/Dancing_Apart/blob/master/images/arduino_code.jpg)


# Hardware 

Materials used:
Stepper motor Nema 17 Bipolar 
Timing Belt 6 mm
Pulley 6 mm
Jumper wires
Arduino UNO
Shield stepper motor
Servo motor
![alt text](https://github.com/cedricdroogmans/Dancing_Apart/blob/master/images/arduino.jpg)
![alt text](https://github.com/cedricdroogmans/Dancing_Apart/blob/master/images/drawbot.jpg)

# Use

1) start server   (node.js)
2) tunnel port    (ngrok)
3) open serialport (p5.serialcontrol)
4) start dancer client side (https://editor.p5js.org/Cedric.Droogmans/sketches/syienGx0-)
5) start DrawBot client side (https://editor.p5js.org/Cedric.Droogmans/sketches/MUZqmLO2C)
