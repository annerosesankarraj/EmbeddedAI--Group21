#define GPS_RX_PIN 44  // GPS TX → ESP32 RX
#define GPS_TX_PIN 43  // GPS RX ← ESP32 TX

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // Remap Serial1 to your pins:
  Serial1.begin(
    9600,            // GPS baud
    SERIAL_8N1,      // data format
    GPS_RX_PIN,      // RX pin (ESP32 reads here)
    GPS_TX_PIN       // TX pin (ESP32 writes here)
  );
}

void loop() {
  // PC → GPS
  if (Serial.available()) {
    Serial1.write( Serial.read() );
  }
  // GPS → PC
  if (Serial1.available()) {
    Serial.write( Serial1.read() );
  }
}
