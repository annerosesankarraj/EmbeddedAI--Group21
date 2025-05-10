#include <ESP32Servo.h>

const int servoPin = 1;   // change if you wired to a different pin
Servo myServo;

void setup() {
  // Attach the servo to the pin, with pulse range 500–2500 µs
  myServo.setPeriodHertz(50);           // 50 Hz for SG90
  myServo.attach(servoPin, 500, 2500);  // min/max pulse widths
}

void loop() {
  // Sweep from 0° to 180°
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);
    delay(15);   // ~15 ms gives a ~2 s sweep; adjust for speed
  }

  delay(500);    // pause at 180°

  // Sweep from 180° back to 0°
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    delay(15);
  }

  delay(500);    // pause at 0°
}
