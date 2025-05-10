#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  // Initialize I2C on GPIO8 (SDA) and GPIO9 (SCL)
  Wire.begin(8, 9);

  Serial.println("Adafruit MPU6050 test!");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }
  Serial.println("MPU6050 Found!");

  // (Optional) Configure sensor ranges & filter
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println();
}

void loop() {
  /* Get new sensor events */
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  /* Print out accelerometer */
  Serial.print  ("Accel (m/s²): X=");
  Serial.print  (accel.acceleration.x, 3);
  Serial.print  (", Y=");
  Serial.print  (accel.acceleration.y, 3);
  Serial.print  (", Z=");
  Serial.print  (accel.acceleration.z, 3);

  /* Print out gyro */
  Serial.print  ("   Gyro (rad/s): X=");
  Serial.print  (gyro.gyro.x, 3);
  Serial.print  (", Y=");
  Serial.print  (gyro.gyro.y, 3);
  Serial.print  (", Z=");
  Serial.print  (gyro.gyro.z, 3);

  /* Print out temperature */
  Serial.print  ("   Temp (°C)=");
  Serial.println(temp.temperature, 1);

  delay(500);
}
