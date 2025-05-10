#include <Arduino.h>

#define TRIG_PIN 18   // Trigger pin connected to GPIO18
#define ECHO_PIN 19   // Echo pin connected to GPIO19

// Motor A wiring (OUT1, OUT2)
const int IN1 = 35;  // PWM pin for Motor A
const int IN2 = 33;  // Direction pin for Motor A

unsigned long startTime;
unsigned long lastAvoidTime = 0;

// Motor B wiring (OUT3, OUT4)
const int IN3 = 34;  // PWM pin for Motor B
const int IN4 = 23;  // Direction pin for Motor B

const int servoPin = 25;  // PWM pin for the servo

int dutyCycle = 0;

const int PWMFreq       = 50;   // 50 Hz for servo control
const int PWMChannel    = 0;
const int PWMResolution = 12;   // 12-bit resolution (0–4095)

// Stop both motors
void stopMotors() {
  ledcWrite(0, 0);  // Stop Motor A
  ledcWrite(1, 0);  // Stop Motor B
}

// Drive both motors forward
void forwardMotors() {
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(0, 50);  // Start Motor A
  ledcWrite(1, 50);  // Start Motor B
}

// Drive both motors backward
void backwardMotors() {
  digitalWrite(IN2, HIGH);  // Reverse Motor A
  digitalWrite(IN4, HIGH);  // Reverse Motor B
  ledcWrite(0, 25);         // Reverse Motor A
  ledcWrite(1, 25);         // Reverse Motor B
}

// Turn left (Motor A forward, Motor B reverse)
void turnLeft() {
  digitalWrite(IN2, LOW);   // Motor A forward
  digitalWrite(IN4, HIGH);  // Motor B reverse
  ledcWrite(0, 150);        // Spin Motor A
  ledcWrite(1, 150);        // Spin Motor B
}

// Set servo to a given angle (0–180°)
void setServoAngle(int angle) {
  angle = constrain(angle, 0, 180);
  int duty = map(angle, 0, 180, 204, 409);  // Map angle to 1 ms–2 ms pulse width
  ledcWrite(PWMChannel, duty);
}

void setup() {
  // Initialize servo PWM channel
  ledcSetup(PWMChannel, PWMFreq, PWMResolution);
  ledcAttachPin(servoPin, PWMChannel);
  ledcWrite(PWMChannel, dutyCycle);

  startTime = millis();
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IN2, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize motor PWM channels
  ledcSetup(0, 1000, 8);
  ledcAttachPin(IN1, 0);
  ledcSetup(1, 1000, 8);
  ledcAttachPin(IN3, 1);

  forwardMotors();  // Start moving forward immediately
}

void loop() {
  long duration;
  float distance;

  // Send a 10 µs HIGH pulse to trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pulse duration (timeout 30 ms)
  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Obstacle avoidance: check every 5 s with a 6 s cooldown
  if (millis() - startTime > 5000 && millis() - lastAvoidTime > 6000) {
    if (distance > 5 && distance < 40) {
      lastAvoidTime = millis();
      Serial.println("Too close! Stopping...");
      stopMotors();
      delay(3000);    // Stop for 3 s

      backwardMotors();
      delay(3000);    // Reverse for 3 s

      Serial.println("Turning left...");
      turnLeft();
      delay(3000);    // Turn left for 3 s

      forwardMotors();  // Resume forward motion
    }
  }

  delay(500);

  // Sweep servo: 0°, 90°, 180°
  setServoAngle(0);   // Move servo to 0°
  delay(1000);
  setServoAngle(90);  // Move servo to 90°
  delay(1000);
  setServoAngle(180); // Move servo to 180°
  delay(1000);
}
