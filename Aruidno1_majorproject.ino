
// Potentiometer goes from +150 [degrees] to -150 [degrees]
// for a total of 300 [degrees] but less than 360 [degrees]

/*Conventions 
  - Orange (right lead) to power, Blue (left) to GND, Yellow to Analog 
  - Increasing voltage with increasing angle (BUT voltage readings are dependent on source) 
  - Base facing front when in neutral (theta = 0) position 
  - FORWARD to positive angles and increasing voltages; BACKWARD to negative angles and decreasing voltages 
  - FORWARD red positive, white ground; BACKWARD red negative, white ground 
  - Red to Black, White to Orange for correct FORWARD and BACKWARD movement 
  - Joints 1 and 2 have positive angles going backward but the extender has positive angles going forward (?)
*/

// Boilerplate Code
#include <Wire.h>   
#include <Adafruit_MotorShield.h>
#include <Arduino.h>

// Initialize a motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Define the motors for the arm joints and base
Adafruit_DCMotor *motorBase = AFMS.getMotor(1);    // Motor 1 (Base rotation)
Adafruit_DCMotor *motorJoint1 = AFMS.getMotor(2);  // Motor 2 (Joint 1)
Adafruit_DCMotor *motorJoint2 = AFMS.getMotor(3);  // Motor 3 (Joint 2)
Adafruit_DCMotor *motorJoint3 = AFMS.getMotor(4);  // Motor 4 (Extending/retracting)

// Constant Variables for the Motor Potentiometer Pins
const int potM1 = A8;
const int potM2 = A9;
const int potM3 = A10;
const int potM4 = A11;
const int potM5 = A12;
int targetM3 = 120;
int targetMB = 0;

//***********************************************************************//

uint8_t reverseDirection(uint8_t currentDirection) {
  if (currentDirection == FORWARD) {
    currentDirection = BACKWARD;
  } else {
    currentDirection = FORWARD;
  }
  return currentDirection;
}

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

  switch (potPin) {
    case A8:
      {  // Base, Motor 1
        currentMotor = motorBase;

        break;
      }
    case A9:
      {  // Joint 1, Motor 2
        currentMotor = motorJoint1;

        break;
      }
    case A10:
      {  // Joint 2, Motor 3
        currentMotor = motorJoint2;
        MIN_ANGLE = 150;  //Potentiometer placement correction
        MAX_ANGLE = -150;
        break;
      }
    case A11:
      {  // Joint 3, Motor 4
        currentMotor = motorJoint3;
        MIN_ANGLE = 150;  //Potentiometer placement correction
        MAX_ANGLE = -150;
        break;
      }
    case A12:
      {
        break;
      }
  }

  // Get the initial angle
  currentAngle = map(analogRead(potPin), POT_MIN, POT_MAX, MIN_ANGLE, MAX_ANGLE);  // Potentiometer values to angles
  currentSpeed = map(abs(targetAngle - currentAngle), 0, 250, 70, 255);
  if (targetAngle > currentAngle) {
    currentDirection = FORWARD;
  } else {
    currentDirection = BACKWARD;
  }
  currentMotor->setSpeed(currentSpeed);
  currentMotor->run(currentDirection);
  //delay(500);

  do {
    // Serial.println("Current Angle = " + String(currentAngle));
    // Serial.println("abs(targetAngle - currentAngle) = " + String(abs(targetAngle - currentAngle)));

    // The algorithm below should cause the system to 'chase' the target point
    if (targetAngle > currentAngle) {
      currentDirection = FORWARD;
      // Serial.println("Current motor direction = " + String(currentDirection));
      currentMotor->run(currentDirection);
      delay(150);
    } else {
      currentDirection = BACKWARD;
      // Serial.println("Current motor direction = " + String(currentDirection));
      currentMotor->run(currentDirection);
      delay(150);
    }

    

    // As the gap changes, the speed decreases
    // Max Speed of .run() is 255 for 'full throttle'
    currentAngle = map(analogRead(potPin), POT_MIN, POT_MAX, MIN_ANGLE, MAX_ANGLE);
    currentSpeed = map(abs(targetAngle - currentAngle), 0, 300, 80, 255);  // degrees to motor speed
    currentMotor->setSpeed(currentSpeed);
    // Serial.println("Current Speed = " + String(currentSpeed));

  } while (abs(targetAngle - currentAngle) > 6); // initially = 0 
  currentMotor->run(RELEASE);
  // Serial.println("Motor has reached target position");
}

void standardPosition() {
  moveMotor(potM1,-10);  //A8 for base
  moveMotor(potM2);
  moveMotor(potM3, 120);  //A10 for Joint 2
  moveMotor(potM4, -40);
}

//***********************************************************************//

void setup() {
  // Start the motor shield
  AFMS.begin();

  // Start the Serial Monitor connection
  Serial.begin(9600);
  delay(1000);

  standardPosition();
  //   for(int i = 0; i < 8; i++)
  // {
  //   Serial.println("Increment" + String(i));
  //   targetMB=targetMB+2;
  //   moveMotor(potM1, targetMB);
  // }

  // // Perform the pick and drop action 5 times without feedback
  // /*
  // for (int i = 0; i < 5; i++) {
  //   pickBlock();    // Pick up the block from Point A
  //   moveToPointB(); // Move to Point B
  //   dropBlock();    // Drop the block at Point B
  //   returnToPointA(); // Return to Point A to pick the block again
  //   delay(1000);     // Wait for 1 second before repeating the action
  // }
  // */

  // for (int i = 0; i < 5; i++) {
  //   //Move to Position A
  //   moveMotor(potM1, 45);
  //   moveMotor(potM4, 45);
  //   moveMotor(potM2, -55);
  //   moveMotor(potM3, 120);
  //   moveMotor(potM3, 90);
  //   moveMotor(potM2, 40);
  //   moveMotor(potM4, -10);
  //   //Move the Position B
  //   moveMotor(potM1, -45);
  //   moveMotor(potM4, 45);
  //   moveMotor(potM2, -55);
  //   moveMotor(potM3, 120);
  //   moveMotor(potM3, 90);
  //   moveMotor(potM2, 40);
  //   moveMotor(potM4, -10);
  // }
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1'){
  standardPosition();
  moveMotor(potM3, 95);
  delay(1000);
  moveMotor(potM1, -103);
  delay(1000);
  moveMotor(potM2, -38);
  delay(1000);
     Serial.println("2");
    }
    if (command == '3'){
  moveMotor(potM3,60);
  moveMotor(potM1,-10);  //A8 for base
  moveMotor(potM2);
  moveMotor(potM4, -40);
  moveMotor(potM2, -55);
  moveMotor(potM3, 100);
  delay(1000);
  Serial.println("4");
    }
    if (command == '5'){
  standardPosition();
  delay(5000); //wait for 2 minutes
  Serial.println("7");
    }
    if (command == '7'){
  standardPosition();
  moveMotor(potM1,-15);
  delay(1000);
      moveMotor(potM3,95);
  delay(1000);
    moveMotor(potM2, -60);
  delay(1000);
  Serial.println("8");
    }
    if (command == '9'){
       moveMotor(potM1,-10);  //A8 for base
  moveMotor(potM2);
  moveMotor(potM3, 110);  //A10 for Joint 2
  moveMotor(potM4, -40);
      moveMotor(potM1, 50);
      delay(1000);
      moveMotor(potM2, 25);
      delay(1000);
      Serial.println("0");
    }
}
}


// #include <Servo.h>
// Servo myservo;

// void setup() {
//     Serial.begin(9600);
//     myservo.attach(9);  // Set based on your connection
// }

// void loop() {
//     if (Serial.available() > 0) {
//         String command = Serial.readStringUntil('\n');
//         if (command.startsWith("MOVE")) {
//             int pos1 = command.substring(5, 7).toInt();
//             int pos2 = command.substring(8, 10).toInt();
//             myservo.write(pos1);  // Example, adjust as needed
//             delay(1000);          // Wait for the arm to move
//             myservo.write(pos2);
//         }
//     }
// }
