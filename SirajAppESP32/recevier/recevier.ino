#include <esp_now.h>
#include <BluetoothSerial.h>
#include <WiFi.h>

BluetoothSerial SerialBT;

int waterLevel1 = 0;
int waterLevel2 = 0;

// Callback function that will be executed when data is received
void onDataReceive(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int len) {
  if (len == sizeof(waterLevel1)) {
    memcpy(&waterLevel1, data, sizeof(waterLevel1));
  } else {
    Serial.println("Received data length mismatch");
  }

}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function for receiving data
  esp_now_register_recv_cb(onDataReceive);
}

void loop() {
  delay(1000); // Delay for readability, adjust as necessary

  waterLevel2 = analogRead(34);

  // Send the water level data via Bluetooth
  if (SerialBT.hasClient()) {
    SerialBT.println(waterLevel1);
    SerialBT.println(waterLevel2);
  }

  Serial.print("WL1:");
  Serial.println(waterLevel1);
  Serial.print("WL2:");
  Serial.println(waterLevel2);
}