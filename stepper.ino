//Stepper Controller


//#include <Arduino.h>
//
//// Define the step and direction pins for the A4988 driver
//#define STEP_PIN A0
//#define DIR_PIN A1
//#define EN_PIN 38
//
//// Define the steps per revolution for your stepper motor
//#define STEPS_PER_REVOLUTION 2000 // Change this to match your motor's specifications
//
//void setup() {
//  // Set the step and direction pins as outputs
//  pinMode(STEP_PIN, OUTPUT);
//  pinMode(DIR_PIN, OUTPUT);
//  pinMode(EN_PIN, OUTPUT);
//  digitalWrite(EN_PIN, LOW);
//}
//
//void loop() {
//  // Set the direction (HIGH for clockwise, LOW for counterclockwise)
//  digitalWrite(DIR_PIN, LOW); // Change this to LOW to reverse direction
//
//  // Step the motor one step at a time
//  for (int i = 0; i < STEPS_PER_REVOLUTION; i++) {
//    digitalWrite(STEP_PIN, HIGH);
//    delayMicroseconds(500); // Adjust the delay for your motor's speed
//    digitalWrite(STEP_PIN, LOW);
//    delayMicroseconds(500); // Adjust the delay for your motor's speed
//  }
//
//  // Pause for a moment before reversing direction
//  delay(1000);
//
//  // Reverse direction (LOW for clockwise, HIGH for counterclockwise)
//  digitalWrite(DIR_PIN, HIGH); // Change this to HIGH to reverse direction
//
//  // Step the motor one step at a time in the opposite direction
//  for (int i = 0; i < STEPS_PER_REVOLUTION; i++) {
//    digitalWrite(STEP_PIN, HIGH);
//    delayMicroseconds(15); // Adjust the delay for your motor's speed
//    digitalWrite(STEP_PIN, LOW);
//    delayMicroseconds(15); // Adjust the delay for your motor's speed
//  }
//
//  // Pause for a moment before repeating
//  delay(1);
//}

//  {'row': 112, 'motor1': 20.0, 'motor2': 20.0, 'motor3': 20.0}

#include <AccelStepper.h>
#include <ArduinoJson.h>

// Define the capacity for the JSON object based on your data structure
const size_t capacity = JSON_OBJECT_SIZE(5); // Adjust based on your data structure

// Create a JSON object
DynamicJsonDocument jsonDoc(200);

// Define the step and direction pins for the stepper motor
#define STEP_PIN_M1 A0
#define DIR_PIN_M1 A1
#define EN_PIN_M1 38

#define STEP_PIN_M2 A6
#define DIR_PIN_M2 A7
#define EN_PIN_M2 A2

#define STEP_PIN_M3 46
#define DIR_PIN_M3 48
#define EN_PIN_M3 A8

// Create an instance of the AccelStepper class
AccelStepper stepper1(1, STEP_PIN_M1, DIR_PIN_M1);
AccelStepper stepper2(1, STEP_PIN_M2, DIR_PIN_M2);
AccelStepper stepper3(1, STEP_PIN_M3, DIR_PIN_M3);
String incomingData = "";
bool newData = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(EN_PIN_M1, OUTPUT);
  digitalWrite(EN_PIN_M1, LOW);
  pinMode(EN_PIN_M2, OUTPUT);
  digitalWrite(EN_PIN_M2, LOW);
  pinMode(EN_PIN_M3, OUTPUT);
  digitalWrite(EN_PIN_M3, LOW);
  String incomingData = "";
  bool newData = false;
  
  stepper1.setMaxSpeed(384*50);      // Set the maximum speed in steps per second
  stepper1.setAcceleration(384*50);    // Set the acceleration in steps per second^2
  stepper1.setSpeed(384*50); // 24 * 16 = 384
  //stepper1.moveTo(5000);           // Set the target position (3600 steps = 360 degrees)
  stepper2.setMaxSpeed(384*50);      // Set the maximum speed in steps per second
  stepper2.setAcceleration(384*50);    // Set the acceleration in steps per second^2
  stepper2.setSpeed(384*50); // 24 * 16 = 384
  
  stepper3.setMaxSpeed(384*50);      // Set the maximum speed in steps per second
  stepper3.setAcceleration(384*50);    // Set the acceleration in steps per second^2
  stepper3.setSpeed(384*50); // 24 * 16 = 384
}

void loop() {

  if (Serial.available() > 0) {
//     String serialData = Serial.readStringUntil('\n');
     char c = Serial.read();
    if (c == '\n') {
      // End of the line, send the data through ESP-NOW
      if (newData) {
        // Send the data to the receiver
//        esp_now_send(receiverMac, (uint8_t *)incomingData.c_str(), incomingData.length());
        Serial.println("Sending data: " + incomingData);
        json_data(incomingData);
        incomingData = ""; // Clear the data buffer
        newData = false;
      }
    } else {
      // Add the character to the incoming data buffer
      incomingData += c;
      newData = true;
    }
     

}
  
  Serial.print(stepper1.distanceToGo());
  Serial.print("  ");
  Serial.print(stepper2.distanceToGo());
  Serial.print("  ");
  Serial.print(stepper3.distanceToGo());
  Serial.println();
  
   
  stepper1.run(); // Run the stepper motor
  stepper2.run();
  stepper3.run();
}

void stepperRun(int M1,int M2,int M3){
    stepper1.moveTo(M1);
    stepper2.moveTo(M2);
    stepper3.moveTo(M3);
}

void json_data(String serialData){
       DeserializationError error = deserializeJson(jsonDoc, serialData);
      if (!error) {
      // Check if the parsed JSON has the expected fields
      if (jsonDoc.containsKey("row") && jsonDoc.containsKey("motor1") && jsonDoc.containsKey("motor2") && jsonDoc.containsKey("motor3")) {
        // Access and print individual JSON values
        int rowValue = jsonDoc["row"];
        float motor1Value = jsonDoc["motor1"];
        float motor2Value = jsonDoc["motor2"];
        float motor3Value = jsonDoc["motor3"];

        // Do something with the parsed values
        Serial.print("Parsed JSON Data - row: ");
        Serial.println(rowValue);
        Serial.print("motor1: ");
        Serial.println(motor1Value);
        Serial.print("motor2: ");
        Serial.println(motor2Value);
        Serial.print("motor3: ");
        Serial.println(motor3Value);
        if (stepper1.distanceToGo()== stepper2.distanceToGo()== stepper3.distanceToGo() == 0) {
    // If the stepper motor has reached the target position, reverse direction
    // stepper1.moveTo(-stepper.currentPosition());
    stepperRun(motor1Value,motor1Value,motor1Value);

  }
  else{
//    stepper1.moveTo(5000);
//    stepper2.moveTo(5000);
    }
      } else {
        Serial.println("Invalid JSON format or missing fields");
      }
    } else {
      Serial.println("Error parsing JSON");
    }
}