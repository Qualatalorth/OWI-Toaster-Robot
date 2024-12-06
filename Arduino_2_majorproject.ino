#include <AccelStepper.h>

// Define pins for step and direction
#define STEP_PIN 9
#define DIR_PIN 8

// Create an instance of the AccelStepper class
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// void setup() {
//   // Set max speed and acceleration
//   stepper.setMaxSpeed(4000);  // Max speed in steps per second
//   stepper.setAcceleration(2000);  // Acceleration in steps/second^2

//   Serial.begin(9600);
//   Serial.println("Stepper motor test with TB6600");
// }

// void loop() {
//   // Move 1000 steps forward
//   if (Serial.available() > 0) {
//     char command = Serial.read();
//     Serial.print("Received: ");
//     Serial.println(command); // Print the received command
//     if (command == '1'){
//   stepper.moveTo(-25000);
//   while (stepper.distanceToGo() != 0) {
//     stepper.run();
//   }
//   delay(1000);  // Pause for a moment

//   // Move 1000 steps backward
//   stepper.moveTo(0);
//   while (stepper.distanceToGo() != 0) {
//     stepper.run();
//   }

//   delay(1000);  // Pause for a moment
//     }
//   }
// }

/* For gripper: White to Black, Red to Yellow 

*/ 

// Boilerplate Code
#include <Wire.h>   
#include <Adafruit_MotorShield.h>
#include <Arduino.h>

// Initialize a motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Define the motors for the arm joints and base
Adafruit_DCMotor *motorGripper = AFMS.getMotor(3);  // Motor 1 (Extending/retracting) 

// Constant Variables for the Motor Potentiometer Pins
const int potM5 = A8;

// Modified moveMotor function for the gripper 
void moveMotor(int potPin, int targetAngle = 0) {
  // Calibration Variables and Values
  int MIN_ANGLE = -150;
  int MAX_ANGLE = 150;
  int POT_MIN = 0;
  int POT_MAX = 1023;
  uint8_t currentDirection = RELEASE;  // The initial direction
  int currentAngle = 0;
  int currentSpeed = 0; 
  Adafruit_DCMotor *currentMotor; 
  currentMotor = motorGripper; 

  // Get the initial angle
  currentAngle = map(analogRead(potPin), POT_MIN, POT_MAX, MIN_ANGLE, MAX_ANGLE);  // Potentiometer values to angles
  currentSpeed = map(abs(targetAngle - currentAngle), 0, 250, 90, 255);
  if (targetAngle > currentAngle) {
    currentDirection = FORWARD;
  } else {
    currentDirection = BACKWARD;
  }
  currentMotor->setSpeed(currentSpeed);
  currentMotor->run(currentDirection);
  //delay(500);

  do {
    //Serial.println("Current Angle = " + String(currentAngle));
    //Serial.println("abs(targetAngle - currentAngle) = " + String(abs(targetAngle - currentAngle)));

    // The algorithm below should cause the system to 'chase' the target point
    if (targetAngle > currentAngle) {
      currentDirection = FORWARD;
      //Serial.println("Current motor direction = " + String(currentDirection));
      currentMotor->run(currentDirection);
      delay(150);
    } else {
      currentDirection = BACKWARD;
      //Serial.println("Current motor direction = " + String(currentDirection));
      currentMotor->run(currentDirection);
      delay(150);
    }

    

    // As the gap changes, the speed decreases
    // Max Speed of .run() is 255 for 'full throttle'
    currentAngle = map(analogRead(potPin), POT_MIN, POT_MAX, MIN_ANGLE, MAX_ANGLE);
    currentSpeed = map(abs(targetAngle - currentAngle), 0, 300, 90, 255);  // degrees to motor speed
    currentMotor->setSpeed(currentSpeed);
    //Serial.println("Current Speed = " + String(currentSpeed));

  } while (abs(targetAngle - currentAngle) > 6); // initially = 0 
  currentMotor->run(RELEASE);
  // Serial.println("Motor has reached target position");
}
void setup() {
  // Start the motor shield
  AFMS.begin();
  stepper.setMaxSpeed(4000);  // Max speed in steps per second
  stepper.setAcceleration(2000);  // Acceleration in steps/second^2

  Serial.begin(9600);
  // Start the Serial Monitor connection
  Serial.begin(9600);
  delay(1000);

  moveMotor(potM5, 0); 
}

void loop(){
  if (Serial.available() > 0) {
    char command = Serial.read();
    // Serial.print("Received: ");
    // Serial.println(command); // Print the received command
    if (command == '6'){
  stepper.moveTo(-25000);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
  delay(1000);  // Pause for a moment

  // Move 1000 steps backward
  stepper.moveTo(0);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  delay(1000);  // Pause for a moment
    }
  if (command == '2'){
  moveMotor(potM5, -60);
  delay(1000);
  Serial.println("3");
  }
  if (command == '4'){
  moveMotor(potM5, 30);
  delay(1000);
  Serial.println("5");
  }
  if (command == '8'){
  moveMotor(potM5, -60);
  delay(1000);
  Serial.println("9");
  }
   if (command == '0'){
  moveMotor(potM5, 30);
  delay(1000);
  Serial.println("10");
  }
}
}

