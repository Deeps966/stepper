#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "stepper"; // Your AP's SSID
const char* password = "stepper123"; // Your AP's password
int receiverPort = 8000; // Port to listen on

WiFiServer server(receiverPort);

const size_t JSON_BUFFER_SIZE = 256;
DynamicJsonDocument doc(JSON_BUFFER_SIZE);

void setup() {
  Serial.begin(115200);

  // Start ESP32 as an access point
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");

    String jsonData = "";

    // Continuously read data from the Serial port and send it to the client
    while (client.connected()) {
      // Check if data is available on the Serial port
      while (Serial.available()) {
        char c = Serial.read();
        jsonData += c; // Accumulate characters in a string
      }

      // Check if we received a complete JSON message
      if (jsonData.endsWith("\n")) {
        jsonData.trim(); // Remove leading/trailing whitespace
        Serial.println("Received JSON data: " + jsonData);

        // Parse the JSON data
        DeserializationError error = deserializeJson(doc, jsonData);
        if (!error) {
          // Send the parsed JSON back to the client
          serializeJson(doc, client);
          client.println(); // Add a newline after JSON data
          doc.clear();
        } else {
          Serial.println("Error parsing JSON");
        }

        jsonData = ""; // Clear the string for the next JSON message
      }
    }
//
//    Serial.println("Client disconnected");
//    client.stop();
  }
}
