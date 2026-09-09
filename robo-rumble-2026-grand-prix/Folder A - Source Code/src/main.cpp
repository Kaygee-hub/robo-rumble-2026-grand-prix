/**
 * Robo Rumble 2026 — Robo Grand Prix (Autonomous Maze Racing)
 * 4WD Rover High-Speed Wall-Centering & Navigation Firmware
 * 
 * Hardware:
 * - Microcontroller: Arduino Uno / Nano / Mega
 * - Driver: L298N / DRV8833 Dual H-Bridge
 * - Sensors: 3x HC-SR04 Ultrasonic Sensors (Front, Left, Right)
 */

#include <Arduino.h>

// ==========================================
// 1. PIN CONFIGURATIONS
// ==========================================

// Motor Driver Pins (L298N / DRV8833)
const int ENA = 5;   // PWM Speed Control Left Motors
const int IN1 = 4;   // Direction Left Forward
const int IN2 = 7;   // Direction Left Reverse

const int ENB = 6;   // PWM Speed Control Right Motors
const int IN3 = 8;   // Direction Right Forward
const int IN4 = 12;  // Direction Right Reverse

// Ultrasonic Sensor Pins
const int TRIG_FRONT = 2;
const int ECHO_FRONT = 3;

const int TRIG_LEFT  = 9;
const int ECHO_LEFT  = 10;

const int TRIG_RIGHT = 11;
const int ECHO_RIGHT = 13;

// ==========================================
// 2. RACING TUNING PARAMETERS
// ==========================================
const int MAX_SPEED       = 255;  // Full speed PWM (0 - 255)
const int BASE_SPEED      = 220;  // Cruising speed on straightaways
const int MIN_SPEED       = 100;  // Minimum wheel PWM during tight turns

const int FRONT_THRESHOLD = 25;   // Critical distance (cm) to trigger cornering
const int SENSOR_TIMEOUT  = 10000; // Timeout in microseconds (~170cm max distance limit)

// PD (Proportional-Derivative) Control Gains
float Kp = 6.5;                   // Steering aggression (Increase if weaving too late)
float Kd = 3.2;                   // Steering damping (Increase if rover oscillates/wobbles)

// Steering variables
float currentError = 0.0;
float lastError    = 0.0;

// Sensor Distance Variables (cm)
float distFront, distLeft, distRight;

// ==========================================
// 3. FUNCTION DECLARATIONS
// ==========================================
float getDistance(int trigPin, int echoPin);
void readAllSensors();
void setMotorSpeeds(int leftSpeed, int rightSpeed);
void executePivotTurn();

// ==========================================
// 4. SETUP
// ==========================================
void setup() {
  // Motor Pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic Pins
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  // Initial Motor State: Stopped
  setMotorSpeeds(0, 0);

  // Short delay before startup run
  delay(1000);
}

// ==========================================
// 5. MAIN CONTROL LOOP
// ==========================================
void loop() {
  readAllSensors();

  // -------------------------------------------------------------
  // CONDITION 1: OBSTACLE AHEAD -> HARD CORNERING / PIVOT
  // -------------------------------------------------------------
  if (distFront < FRONT_THRESHOLD && distFront > 0) {
    executePivotTurn();
  } 
  // -------------------------------------------------------------
  // CONDITION 2: CLEAR AHEAD -> HIGH-SPEED PD WALL-CENTERING
  // -------------------------------------------------------------
  else {
    // Determine wall centering strategy based on available side walls
    if (distLeft > 0 && distRight > 0 && distLeft < 60 && distRight < 60) {
      // Both walls detected: Center in middle of corridor
      currentError = distLeft - distRight;
    } 
    else if (distLeft > 0 && distLeft < 60) {
      // Only Left wall detected: Maintain 20 cm distance from Left wall
      currentError = (distLeft - 20.0) * 1.5;
    } 
    else if (distRight > 0 && distRight < 60) {
      // Only Right wall detected: Maintain 20 cm distance from Right wall
      currentError = (20.0 - distRight) * 1.5;
    } 
    else {
      // Open space / No close side walls: Drive straight forward
      currentError = 0.0;
    }

    // Calculate PD Steering Correction
    float derivative = currentError - lastError;
    float steeringAdjustment = (Kp * currentError) + (Kd * derivative);
    lastError = currentError;

    // Apply differential PWM speed control
    int leftMotorPWM  = BASE_SPEED - steeringAdjustment;
    int rightMotorPWM = BASE_SPEED + steeringAdjustment;

    // Constrain PWM values to safe hardware limits
    leftMotorPWM  = constrain(leftMotorPWM, MIN_SPEED, MAX_SPEED);
    rightMotorPWM = constrain(rightMotorPWM, MIN_SPEED, MAX_SPEED);

    // Drive forward with differential steering
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    setMotorSpeeds(leftMotorPWM, rightMotorPWM);
  }
}

// ==========================================
// 6. HARDWARE & SENSOR HELPER FUNCTIONS
// ==========================================

/**
 * Fast ultrasonic distance reading routine with tight pulse timeout.
 */
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read pulse width with microsecond timeout (prevents code stall)
  long duration = pulseIn(echoPin, HIGH, SENSOR_TIMEOUT);

  if (duration == 0) {
    return 150.0; // Return max distance out of range
  }
  
  return (duration * 0.0343) / 2.0; // Convert microseconds to cm
}

/**
 * Batch read all three range sensors
 */
void readAllSensors() {
  distFront = getDistance(TRIG_FRONT, ECHO_FRONT);
  distLeft  = getDistance(TRIG_LEFT, ECHO_LEFT);
  distRight = getDistance(TRIG_RIGHT, ECHO_RIGHT);
}

/**
 * Direct PWM Output Helper
 */
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

/**
 * High-speed wall escape / cornering decision execution
 */
void executePivotTurn() {
  // Determine open side (Turn toward the side with more space)
  if (distLeft > distRight) {
    // Spin Left in place
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  } else {
    // Spin Right in place
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    setMotorSpeeds(MAX_SPEED, MAX_SPEED);
  }

  // Pivot until front path is cleared
  while (getDistance(TRIG_FRONT, ECHO_FRONT) < FRONT_THRESHOLD + 10) {
    delay(10);
  }
}