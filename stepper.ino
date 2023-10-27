#include <ArduinoJson.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

const int motor1StepPin = 2; // Define the step pin for motor 1
const int motor1DirPin = 3;  // Define the direction pin for motor 1
AccelStepper motor1(1, motor1StepPin, motor1DirPin);

const int motor2StepPin = 4; // Define the step pin for motor 2
const int motor2DirPin = 5;  // Define the direction pin for motor 2
AccelStepper motor2(1, motor2StepPin, motor2DirPin);

const int motor3StepPin = 6; // Define the step pin for motor 3
const int motor3DirPin = 7;  // Define the direction pin for motor 3
AccelStepper motor3(1, motor3StepPin, motor3DirPin);

MultiStepper multiStepper;

void setup()
{
  Serial.begin(115200);

  // Set motor speed and acceleration
  motor1.setMaxSpeed(1000);
  motor1.setAcceleration(500);
  motor2.setMaxSpeed(1000);
  motor2.setAcceleration(500);
  motor3.setMaxSpeed(1000);
  motor3.setAcceleration(500);

  multiStepper.addStepper(motor1);
  multiStepper.addStepper(motor2);
  multiStepper.addStepper(motor3);
}

void loop()
{
  if (Serial.available())
  {
    String jsonStr = Serial.readStringUntil('\n'); // Read JSON data until a newline character

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, jsonStr);

    if (!error)
    {
      double a1 = doc["a1"];
      double a2 = doc["a2"];
      double d3 = doc["d3"];

      // Calculate steps based on your motor configuration and desired angles/displacement
      // long steps1 = calculateStepsForMotor1(a1);
      // long steps2 = calculateStepsForMotor2(a2);
      // long steps3 = calculateStepsForMotor3(d3);

      // Not tested but given by ChatGPT
      // Calculate the number of steps needed for each motor
      long steps1 = (a1 / 360.0) * 5000;
      long steps2 = (a2 / 360.0) * 5000;
      long steps3 = (d3 / 15.0) * 5000; // 15mm corresponds to 15 degrees for a linear motion

      multiStepper.moveTo(steps1, steps2, steps3);
      multiStepper.runSpeedToPosition();
    }
    else
    {
      Serial.println("Error parsing JSON data");
    }
  }
}

long calculateStepsForMotor1(double a1)
{
  // Implement your own logic to calculate steps for motor1 based on angle (a1)
  // Return the calculated steps
}

long calculateStepsForMotor2(double a2)
{
  // Implement your own logic to calculate steps for motor2 based on angle (a2)
  // Return the calculated steps
}

long calculateStepsForMotor3(double d3)
{
  // Implement your own logic to calculate steps for motor3 based on displacement (d3)
  // Return the calculated steps
}