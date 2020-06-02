//variables for socket connection
var socket;


//varbiables for port connection
var serial; //variable to hold an instance of the serial port library
var portName = 'COM3'; //fill in with YOUR port

var myString;
var lf = 10;
var nextPoint;

var Point_Array = [];

let delay;

var actualTime;
var intervalTime = 100;

var options = { baudrate: 115200}; // change the data rate to whatever you wish





//------------------------------------------------------------
function setup() {
  createCanvas(640, 480);
  background(0);
  
  serial = new p5.SerialPort(); //a new instance of serial port library
  //open our serial port
  serial.open(portName, options);
  serial.on('data', serialEvent);     // callback for when new data arrives
  
  socket = io.connect('https://c93fae78500a.ngrok.io');
  socket.on('mouse',

    function(data) {
      //console.log("Got: " + data.x + " " + data.y);
      // Draw a blue circle
      fill(0,0,255);
      noStroke();
      ellipse(data.x, data.y, 2, 2);
      //serial.write("<",data.x,",",data.y,">",);    
      //console.log("<",data.x,",",data.y,">"); 
      Point_Array.push(createVector(data.x,data.y));
      //console.log(Point_Array.length);    
    }
  ); 
  actualTime = millis();
}


//----------------------------------------------------------------
function draw() {
    background('black');
  /*
    while (serial.available() > 0 && millis() > (actualTime + intervalTime)) {
    myString = serial.readLine();
      trim(myString);
      console.log(myString);
    if (myString != null) {
      
      myString = myString.replace('\n',' ');
      myString = myString.replace('\r',' ');
      myString = myString.replace("\\s", "");
      
      //console.log(myString);
      
      
    if (myString == ("A") && Point_Array.length > 0) {
      serial.clear();
      nextPoint = "<"+Point_Array[0].x+","+Point_Array[0].y+">";
      console.log("sended: "+nextPoint);
      serial.write(nextPoint);
      Point_Array.shift();
    }
    }
    actualTime = millis();
  }*/
 
}

function serialEvent() {
    myString = serial.readLine();
    //console.log("1-> "+myString);  
    //trim(myString);
      //myString = myString.replace('"','');
     // console.log(String(myString));
    
      
      
    if (myString == ("A") && Point_Array.length > 0) {
      console.log("-> "+myString); 
      console.log("-> "+Point_Array.length); 
      serial.clear();
      nextPoint = "<"+Point_Array[0].x+","+Point_Array[0].y+">";
      console.log("sended: "+nextPoint);
      serial.write(nextPoint);
      Point_Array.shift();
    }
    
}


