/*
 This sketch gets X/Y/Z values from a Triple Axis Accelerometer Breakout (MMA8452Q), from arduino
 */

import processing.serial.*;
Serial myPort;

float xValue = 0;
float yValue = 0;
float zValue = 0;


void setup() {
  size(600, 600);
  frameRate(12);

  //println(Serial.list());
  String portName = Serial.list()[4];
  myPort = new Serial(this, portName, 9600);
}


void draw() {

  background(0, 0, 0);
  smooth();

  translate(width/2, height/2);
  line(0, height - height/2 - 10, 0, 0 - height/2 + 10);
  line(width - width/2 - 10, 0, 0 - width/2 + 10, 0);
  stroke(255);

  // get the values
  while (myPort.available () > 0) {
    // its > the 0 because its receiving bytes (always more then 0 numbers)
    String myString = myPort.readStringUntil('\n');
    if (myString != null) {
      //println(myString);
      float[] numberValue = float(splitTokens(myString, ","));

      // add new data to the end
      xValue = numberValue[0];
      yValue = numberValue[1];
      zValue = numberValue[2];
    }
  }

  // map and round the values
  float xValueMap = map(xValue, -1, 1, -100, 100);
  int xValueRound = round(xValueMap);

  float yValueMap = map(yValue, -1, 1, -100, 100);
  int yValueRound = round(yValueMap);

  float zValueMap = map(zValue, -1, 1, 0, 40);
  int zValueRound = round(zValueMap);


  // put the values on the screen
  textSize(10);

  text("X:", 0 - width/2 + 10, height/2 - 40); 
  text(xValueRound, 0 - width/2 + 20, height/2 - 40);

  text("Y:", 0 - width/2 + 10, height/2 - 25);
  text(yValueRound, 0 - width/2 + 20, height/2 - 25);

  text("Z:", 0 - width/2 + 10, height/2 - 10);
  text(zValueRound, 0 - width/2 + 20, height/2 - 10);

  ellipse(xValueRound, yValueRound, zValueRound, zValueRound);
  fill(255, 255, 255);


  // send an 'a' to arduino for more bytes 
  myPort.write('a');
}

