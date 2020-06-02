//-------------------------------------------------------------------------------------------
//Libs
#include <AccelStepper.h>
#include <AFMotor.h>
#include <Coordinates.h>
#include <stdio.h>
#include <string.h>


//-------------------------------------------------------------------------------------------
//variables screen and board
int x_Screen = 640;
int y_Screen = 480;

//variables drawing board
float x_DrawingBoard = 1300;
float y_DrawingBoard = 975;

// Home Point
float Global_Home_x = 750;
float Global_Home_y = 50;

// scaled variables for drawing board
float XcoordFromPC_pixels = 320;
float XcoordFromPC_mm = 0;

float YcoordFromPC_pixels = 0;
float YcoordFromPC_mm = 0;


//-------------------------------------------------------------------------------------------
// two stepper motors one on each port
AF_Stepper motor1(200, 1);
AF_Stepper motor2(200, 2);        


//-------------------------------------------------------------------------------------------
// wrappers for the first motor!
void forwardstep1() {  
  motor1.onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  motor1.onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  motor2.onestep(FORWARD, SINGLE);
}
void backwardstep2() {  
  motor2.onestep(BACKWARD, SINGLE);
}


//-------------------------------------------------------------------------------------------
// Motor shield has two motor ports, now we'll wrap them in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);


//-------------------------------------------------------------------------------------------
//for incoming serial data
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];   
boolean newData = false;
boolean newPosition = false;


//-------------------------------------------------------------------------------------------
//read data from serial with start and end markers <>    <102,40><30,292>
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}


//-------------------------------------------------------------------------------------------
//parse data from serial  
void parseData() {                         
    char * strtokIndx;                     
    strtokIndx = strtok(tempChars,",");     
    XcoordFromPC_pixels = atoi(strtokIndx); 
    XcoordFromPC_mm = map(XcoordFromPC_pixels, 0, x_Screen, 0, x_DrawingBoard);
    strtokIndx = strtok(NULL, ",");       
    YcoordFromPC_pixels = atoi(strtokIndx);  
    YcoordFromPC_mm = map(YcoordFromPC_pixels, 0, y_Screen, 0, y_DrawingBoard);

    Serial.print("received: ");
    Serial.print(XcoordFromPC_mm);
    Serial.print(" , ");
    Serial.println(YcoordFromPC_mm);
}


//-------------------------------------------------------------------------------------------
//Coordinates for current and next postion left and right
Coordinates L_currentPosition = Coordinates();
Coordinates L_nextPosition = Coordinates();

Coordinates R_currentPosition = Coordinates();
Coordinates R_nextPosition = Coordinates();


//-------------------------------------------------------------------------------------------
//for calculating steps
float L_XcurrentPosition = Global_Home_x;
float L_YcurrentPosition = Global_Home_y;

float R_XcurrentPosition = Global_Home_x;
float R_YcurrentPosition = Global_Home_y;

float L_R_currentPosition = 0;
float R_R_currentPosition = 0;

float L_R_nextPosition = 0;
float R_R_nextPosition = 0;

float L_R_error = 0;
float R_R_error = 0;

int L_CMDStepper = 0;
int R_CMDStepper = 0;

float RperStep = 0.16;                    // length of belt per step

//-------------------------------------------------------------------------------------------
//calculate CMD stepper 
void calc_steps()
{
  L_currentPosition.fromCartesian(L_XcurrentPosition, L_YcurrentPosition);
  L_R_currentPosition = L_currentPosition.getR();  
  L_nextPosition.fromCartesian(XcoordFromPC_mm, YcoordFromPC_mm);
  L_R_nextPosition = L_nextPosition.getR();
  L_R_error = L_R_nextPosition - L_R_currentPosition;
  L_CMDStepper = L_R_error/RperStep;  
  L_R_currentPosition = L_R_nextPosition;
  //Serial.print("Left steps");
  //Serial.println(L_CMDStepper);

  
  R_currentPosition.fromCartesian(x_DrawingBoard - R_XcurrentPosition, R_YcurrentPosition);
  R_R_currentPosition = R_currentPosition.getR();  
  R_nextPosition.fromCartesian(x_DrawingBoard - XcoordFromPC_mm, YcoordFromPC_mm);
  R_R_nextPosition = R_nextPosition.getR();
  R_R_error = R_R_nextPosition - R_R_currentPosition;
  R_CMDStepper = R_R_error/RperStep;  
  R_R_currentPosition = R_R_nextPosition;
  //Serial.print("right steps");
  //Serial.println(R_CMDStepper);
}


//-------------------------------------------------------------------------------------------
void setup()
{  
Serial.begin(9600);   
requestPos();
}


//-------------------------------------------------------------------------------------------
void loop()
{ 
//-------------------------------------------------------------------------------------------

// read the incoming data as int
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        newData = false;
        newPosition = true;
    }

//-------------------------------------------------------------------------------------------
// activate step calculation if new data arrives    
    if (newPosition == true) {
        calc_steps();
        newPosition = false;
    }

    
//-------------------------------------------------------------------------------------------
// send steps to stepper
  stepper1.setMaxSpeed(800.0);
  stepper1.setAcceleration(100.0);
  stepper1.moveTo(L_CMDStepper);

  stepper2.setMaxSpeed(800.0);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(R_CMDStepper);


//------------------------------------------------------------------------------------------- 
// Stop at when at next position

    if(stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
      Serial.println("next position");
      requestPos();
    }
        

//------------------------------------------------------------------------------------------- 
// run the steppers       
    stepper1.run();
    stepper2.run();
 
}

void requestPos() {
 while (Serial.available() <= 0) {
      Serial.println("A");   // send a capital A
      delay(300);
  }
}
