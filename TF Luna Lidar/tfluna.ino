/*
  TF-Luna LiDAR  ➜  Adafruit ESP32-S2 Feather
  -------------------------------------------------
  Wiring (UART mode, 115 200 baud, 20 Hz frame rate)

      TF-Luna         Feather
      -------         -------
      VCC (5 V)  ↔  USB (5 V)  or  BAT (3 .7-4 .2 V)
      GND        ↔  GND
      TXD        ↔  GPIO16  (labelled RX on the Feather)
      RXD        ↔  GPIO17  (labelled TX on the Feather)
      Pin 5      ↔  *leave floating* (selects UART mode)

  – Requires Bud Ryerson’s “TFMPlus” library (Library Manager).
  – ESP32 has built-in printf support; no printf.h or SoftwareSerial needed.
*/

#include <TFMPlus.h>
TFMPlus tfm;                           // TF-Luna helper object

// Choose whichever free pins you actually wired to
constexpr int RX_PIN = 3;             // sensor TXD → MCU RX
constexpr int TX_PIN = 1;             // sensor RXD ← MCU TX
HardwareSerial LidarSerial(1);         // use UART1

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}   // wait for USB-CDC console (optional)

  // Start the hardware UART that talks to the sensor
  LidarSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(20);                             // give it a breath

  tfm.begin(&LidarSerial);               // hand the port to the library

  // Optional housekeeping ------------------------------------------------
  tfm.sendCommand(SOFT_RESET, 0);        // warm-reset the sensor
  delay(500);                            // allow it to reboot
  tfm.sendCommand(SET_FRAME_RATE, FRAME_20);   // 20 Hz streaming
  // ----------------------------------------------------------------------

  Serial.println(F("\nTF-Luna ready — streaming at 20 Hz"));
}

// variables reused every pass
int16_t dist_cm  = 0;
int16_t strength = 0;
int16_t temp_C   = 0;

void loop() {
  // Ask the library for a fresh 9-byte frame
  if (tfm.getData(dist_cm, strength, temp_C)) {
    Serial.printf("Dist: %4d cm   Flux: %5d   Temp: %2d °C\n",
                  dist_cm, strength, temp_C);
  } else {
    // Checksum or header error: dump raw bytes once for diagnosis
    tfm.printFrame();
  }
  delay(50);          // 20 Hz cadence (50 ms)
}
