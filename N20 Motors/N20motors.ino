#include <Arduino.h>

// Motor A wiring (OUT1, OUT2)
const int IN1 = 13;  // PWM
const int IN2 = 19;  // DIR

// Motor B wiring (OUT3, OUT4)
const int IN3 = 18;  // PWM
const int IN4 = 17;  // DIR

void setup() {
  // Set direction pins as outputs
  pinMode(IN2, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set motor direction: LOW for forward
  // To reverse the motors, swap the wiring instead of setting HIGH
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);

  // Configure PWM channels
  ledcSetup(0, 1000, 8);      // Channel 0: Motor A, 1kHz, 8-bit
  ledcAttachPin(IN1, 0);
  ledcWrite(0, 250);          // Start Motor A

  ledcSetup(1, 1000, 8);      // Channel 1: Motor B
  ledcAttachPin(IN3, 1);
  ledcWrite(1, 250);          // Start Motor B
}

void loop() {
  // Motors keep running
}
