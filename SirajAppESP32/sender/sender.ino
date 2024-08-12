#include <esp_now.h>
#include <WiFi.h>

// Define the pin where the water level sensor is connected
const int waterSensorPin = 34; // ADC1 Channel 6

// MAC address of the receiver ESP32
uint8_t receiverMacAddress[] = {0xCC, 0xDB, 0xA7, 0x2F, 0xD3, 0xEC};

// Structure to hold sensor data
typedef struct {
  int waterLevel;
} SensorData;

SensorData sensorData;

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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

  // Register the send callback function
  esp_now_register_send_cb(onDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read the analog value from the water level sensor
  sensorData.waterLevel = analogRead(waterSensorPin);

  // Print the analog value to the Serial Monitor
  Serial.print("Sending Water Level: ");
  Serial.println(sensorData.waterLevel);

  // Send sensor data to receiver
  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *)&sensorData, sizeof(sensorData));

  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Send failed");
  }

  // No delay for real-time sending, but you can add a small delay if needed for stability
  delay(1000); // Optional: Adjust based on your requirements
}
