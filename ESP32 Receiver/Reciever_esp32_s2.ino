#include <WiFi.h>
#include <esp_now.h>

// replace with your WROOM32’s MAC:
uint8_t transmitterMac[6] = { 0xF4, 0x65, 0x0B, 0x49, 0x27, 0x30 };

void initEspNow() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();                  // no AP
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    while (true) { delay(1000); }
  }
}

// New callback signature in IDF v5+:
void onDataRecv(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
  // turn MAC into printable string
  char macStr[18];
  snprintf(macStr, sizeof(macStr),
           "%02X:%02X:%02X:%02X:%02X:%02X",
           info->src_addr[0], info->src_addr[1], info->src_addr[2],
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);
  Serial.printf("Received %d bytes from %s: ", len, macStr);
  for (int i = 0; i < len; i++) Serial.printf("%02X ", data[i]);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  initEspNow();

  // register the new‐style receive callback
  if (esp_now_register_recv_cb(onDataRecv) != ESP_OK) {
    Serial.println("Failed to register receive callback");
    while (true) { delay(1000); }
  }

  // add your transmitter as a peer
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, transmitterMac, 6);
  peer.channel = 0;         // use current channel
  peer.encrypt = false;
  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Failed to add peer");
    while (true) { delay(1000); }
  }
}

void loop() {
  // nothing to do here—onDataRecv() will fire
}

