#include <WiFi.h>
#include <esp_now.h>

// replace with your Feather S2’s MAC:
uint8_t receiverMac[6] = { 0x60, 0x55, 0xF9, 0xEB, 0xDE, 0x36 };

void initEspNow() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    while (true) { delay(1000); }
  }
}

// New‐style send callback in IDF v5+
void onDataSent(const uint8_t* addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr),
           "%02X:%02X:%02X:%02X:%02X:%02X",
           addr[0], addr[1], addr[2],
           addr[3], addr[4], addr[5]);
  Serial.printf("Sent to %s  status: %s\n",
                macStr,
                status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Transmitter started, sending every 2s…");
  initEspNow();

  // register the new‐style send callback
  if (esp_now_register_send_cb(onDataSent) != ESP_OK) {
    Serial.println("Failed to register send callback");
    while (true) { delay(1000); }
  }

  // add the receiver as a peer
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMac, 6);
  peer.channel = 0;
  peer.encrypt = false;
  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Failed to add peer");
    while (true) { delay(1000); }
  }
}

void loop() {
  // example payload: you can replace with sensor data, etc.
  uint8_t payload[] = { 0xA1, 0xB2, 0xC3, 0xD4 };

  esp_err_t res = esp_now_send(receiverMac, payload, sizeof(payload));
  if (res != ESP_OK) {
    Serial.printf("esp_now_send error: %d\n", res);
  }
  delay(2000);
}

