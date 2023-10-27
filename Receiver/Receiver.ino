#include <WiFi.h>

const char* ssid = "stepper"; // SSID of the sender's AP
const char* password = "stepper123"; // Password of the sender's AP
IPAddress senderIP(192, 168, 4, 1); // IP address of the sender
int senderPort = 8000; // Port to connect to on the sender

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect ESP32 to the sender's AP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // JSON data to be sent
//  String jsonData = "{\"id\":\"row1\", \"m1\":2,\"m2\":3,\"m3\":5}";

  // Connect to the sender
  if (client.connect(senderIP, senderPort)) {
    
    // Read and print the acknowledgment from the sender
    String jsonData = client.readStringUntil('\n');

    if(jsonData != ""){
      Serial.println(jsonData);
      Serial.println("Received JsonData: " + jsonData);
  
//      client.stop(); 
    }
  } else {
    Serial.println("Connection to sender failed");
  }
}
