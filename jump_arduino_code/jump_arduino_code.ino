#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define MAXCS   9
#define MAXDO   12
#define MAXCLK  13

Adafruit_MAX31855 thermocouple(MAXCS);                // For hardware enabled SPI (sets DO to 12 and clk to 13)
//Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);   // For software SPI (any 3 digital I/O pins)

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_StepperMotor *lr = AFMS.getStepper(200, 1); //left-right
Adafruit_StepperMotor *fb = AFMS.getStepper(200, 2); //front-back


int LIN_MOTOR_SPEED = -35535;
int xPos, yPos;

int count;
String commands[10];
int numCommands;

void setup() {
  count = 0;

  Serial.begin(115200);
  Serial.setTimeout(1);

  AFMS.begin();

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(MAXCS, OUTPUT);
  pinMode(MAXDO, INPUT);
  pinMode(MAXCLK, OUTPUT);

  //  raiseZ();     // Raise the linear motor to keep from colliding with anything
  //  resetX();   // Move X axis all the way to the right, then center
  //  resetY();   // Move Y axis all the way to the front, then center
  //  resetZ();     // Lower linear motor until it touches the sample plate, then raise a little

  xPos = 0;
  yPos = 0;

  numCommands = 0;

  addCommand("reset x");
  addCommand("reset y");
  addCommand("reset z");
  
  Serial.println("Begin");
}

void addCommand(String s) {
  commands[numCommands] = s;
  numCommands += 1;
}

void loop() {
  if (yPos > 50) return;

  count += 1;

  String  s;
  s = Serial.readStringUntil(';');

  if (s == "reset z") {
    resetZ();
  } else if (s == "reset x") {
    resetX();
  } else if (s == "reset y") {
    resetY();
  } else if (s == "help") {
    Serial.println("Available commands:");
    for (int i = 0; i < numCommands; i++) {
      Serial.print("\t");
      Serial.println(commands[i]);
    }
  }

  int x, y;

  if (count % 50 == 0) {
    y = 10;
    count = 0;
  } else {
    y = 0;
  }

  if (count < 25) {
    x = 10;
  } else {
    x = -10;
  }

  xPos += x;
  yPos += y;
  //
  //  if (y < 0) {
  //    fb->step(-y, BACKWARD, INTERLEAVE);
  //  }
  //  else if (y > 0) {
  //    fb->step(y, FORWARD, INTERLEAVE);
  //  }
  //  fb->release();
  //
  //  if (x < 0) {
  //    lr->step(-x, BACKWARD, INTERLEAVE);
  //  }
  //  else if (x > 0) {
  //    lr->step(x, FORWARD, INTERLEAVE);
  //  }
  //  lr->release();
  //
  //  lowerZ();

  double c = thermocouple.readCelsius();
  Serial.print("Temperature(C): ");
  Serial.println(c);

  Serial.print("xPos: ");
  Serial.println(xPos);
  Serial.print("yPos: ");
  Serial.println(yPos);

  delay(100);
}

void resetX() {
  while (!digitalRead(6)) {
    lr->step(5, BACKWARD, DOUBLE);
  }
  while (digitalRead(6)) {
    lr->step(5, FORWARD, DOUBLE);
  }
  //  lr->step(500, FORWARD, DOUBLE);
  lr->release();

  xPos = 0;
}

void resetY() {
  while (!digitalRead(7)) {
    fb->step(5, BACKWARD, DOUBLE);
  }
  fb->step(5, FORWARD, DOUBLE);
  while (digitalRead(7)) {
    fb->step(5, FORWARD, DOUBLE);
  }
  //  fb->step(500, FORWARD, DOUBLE);
  fb->release();

  yPos = 0;
}

void raiseZ() {
  resetZ();
  setMotor(LIN_MOTOR_SPEED, true);
  delay(500);
  setMotor(0, false);
}

void lowerZ() {
  while (!digitalRead(5)) {
    setMotor(LIN_MOTOR_SPEED, false);
    delay(1);
  }
  setMotor(0, false);
}

void resetZ() {
  while (!digitalRead(5)) {
    setMotor(LIN_MOTOR_SPEED, false);
    delay(10);
  }
  while (digitalRead(5)) {
    setMotor(LIN_MOTOR_SPEED, true);
    delay(10);
  }
  //  delay(1200);
  setMotor(0, false);
}

void setMotor(int speed, boolean reverse) {
  if (reverse) {
    analogWrite(A1, speed);
    analogWrite(A0, 0);
  } else {
    analogWrite(A0, speed);
    analogWrite(A1, 0);
  }
}
