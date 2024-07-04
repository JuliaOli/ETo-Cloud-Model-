#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#include "dataset.h"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void sendTemperatureAndHumidity(float minTemp, float maxTemp, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"input\": [" + String(minTemp) + ", " + String(maxTemp) + ", " + String(humidity) + "]}";

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected");
  }
}

void loop() {
  for (int i = 0; i < DATASET_SIZE; i++) {
    float minTemp = dataset[i][0];
    float maxTemp = dataset[i][1];
    float humidity = dataset[i][2];

    sendTemperatureAndHumidity(minTemp, maxTemp, humidity);
    delay(1000); // Introduce a delay to avoid overwhelming the server
  }

  // Add a delay after processing the entire dataset
  Serial.println("Completed processing the dataset");
  while (true) {
    delay(10000); // Halt further processing
  }
}
