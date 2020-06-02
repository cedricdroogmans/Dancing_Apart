let video;
let poseNet; 
let poses = [];
let skeletons = [];

let pg;
let wristX;
let wristY;

let pWristX = 320;
let pWristY = 0;

var socket;

var treshold = 30;



function setup() {
  createCanvas(640, 480);
  video = createCapture(VIDEO);
  video.size(width, height);
  pixelDensity(1);
  pg = createGraphics(width, height);
  poseNet = ml5.poseNet(video, modelReady);

  poseNet.on('pose', function(results) {
    poses = results;
  });
  video.hide();
  
  
  socket = io.connect('https://c93fae78500a.ngrok.io');
  socket.on('mouse',

    function(data) {
      console.log("Got: " + data.x + " " + data.y);
      // Draw a blue circle
      fill(0,0,255);
      noStroke();
      ellipse(data.x, data.y, 2, 2);
    }
  );
}

function draw() {
  image(video, 0, 0, width, height);

  image(pg, 0, 0, width, height);

  // We can call both functions to draw all keypoints and the skeletons
  drawKeypoints();
  //drawSkeleton();
}

function drawKeypoints() {
  for (let i = 0; i < min(poses.length, 1); i++) {
    for (let j = 0; j < poses[i].pose.keypoints.length; j++) {
      let keypoint = poses[i].pose.keypoints[j];
      if (keypoint.score > 0.2) {
        if (j == 10) {
          wristX = keypoint.position.x;
          wristY = keypoint.position.y;
          
          if (abs(dist(wristX, wristY, pWristX, pWristY)) > treshold ) {
          pg.stroke(230, 80, 0);
          pg.strokeWeight(5);
          pg.line(wristX, wristY, pWristX, pWristY);
          sendmouse(wristX,wristY);
          pWristX = wristX;
          pWristY = wristY;
          }
          
          
        }
      }
    }
  }
}


function drawSkeleton() {
  for (let i = 0; i < poses.length; i++) {
    for (let j = 0; j < poses[i].skeleton.length; j++) {
      let partA = poses[i].skeleton[j][0];
      let partB = poses[i].skeleton[j][1];
      stroke(255, 0, 0);
      line(partA.position.x, partA.position.y, partB.position.x, partB.position.y);
    }
  }
}


function gotPoses(results) {
  poses = results;
}

function keyPressed() {
  pg.clear();
}

function modelReady() {
  select('#status').html('model Loaded');
}




function sendmouse(xpos, ypos) {
  console.log("sendmouse: " + xpos + " " + ypos);
  var data = {
    x: xpos,
    y: ypos
  };

  socket.emit('mouse',data);
}