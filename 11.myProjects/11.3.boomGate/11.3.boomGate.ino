/*
  Author:
  Learning Intention: Students will use an ultrasonic sensor to control a servo motor

  Student Notes:
  1. Make sure you include flow chart of your logic
  2. Students should aim to implement different logic approaches to the problem

  3. Suggested Grove ports: Ultrasonic D2 (single signal pin), Servo D3
*/

#include <Servo.h>

const int ULTRASONIC_PIN = 2;
const int SERVO_PIN = 3;

const int DETECT_DISTANCE = 20;

const unsigned long CLEAR_DELAY = 3000;
const unsigned long CLOSE_TIME = 1000;

Servo gateServo;

enum GateState {
  WAITING,
  OPENING,
  OPEN,
  CLEAR_DELAY_STATE,
  CLOSING
};

GateState state = WAITING;

unsigned long clearStart = 0;
unsigned long closingStart = 0;

int readDistance() {
  pinMode(ULTRASONIC_PIN, OUTPUT);

  digitalWrite(ULTRASONIC_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(ULTRASONIC_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_PIN, LOW);

  pinMode(ULTRASONIC_PIN, INPUT);

  long duration = pulseIn(ULTRASONIC_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999;
  }

  return duration * 0.034 / 2;
}

bool vehicleDetected(int distance) {
  return distance <= DETECT_DISTANCE;
}

void openGate() {
  gateServo.write(90);
}

void closeGate() {
  gateServo.write(0);
}

void setup() {
  Serial.begin(115200);

  gateServo.attach(SERVO_PIN);
  closeGate();
}

void loop() {

  int distance = readDistance();
  bool vehicle = vehicleDetected(distance);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print("\tState: ");

  switch (state) {

    // ------------------------------------------------
    // VERSION A - SIMPLE LOGIC
    // ------------------------------------------------
    // A simple version would essentially do:
    //
    // if (vehicle) {
    //   openGate();
    // } else {
    //   closeGate();
    // }
    //
    // This is easy to understand but does not handle
    // timing or a vehicle arriving during closing well.
    // ------------------------------------------------


    // ------------------------------------------------
    // VERSION B - STATE-BASED LOGIC
    // ------------------------------------------------

    case WAITING:

      Serial.println("WAITING");

      if (vehicle) {
        openGate();
        state = OPENING;
      }

      break;


    case OPENING:

      Serial.println("OPENING");

      openGate();

      // Servo is commanded to open.
      // Move into OPEN state.
      state = OPEN;

      break;


    case OPEN:

      Serial.println("OPEN");

      openGate();

      // Keep gate open while a vehicle is present.
      if (!vehicle) {
        clearStart = millis();
        state = CLEAR_DELAY_STATE;
      }

      break;


    case CLEAR_DELAY_STATE:

      Serial.println("CLEAR DELAY");

      // If another vehicle arrives during the delay,
      // immediately return to OPEN.
      if (vehicle) {
        openGate();
        state = OPEN;
        break;
      }

      if (millis() - clearStart >= CLEAR_DELAY) {
        closingStart = millis();
        closeGate();
        state = CLOSING;
      }

      break;


    case CLOSING:

      Serial.println("CLOSING");

      // If a vehicle arrives while closing,
      // reopen the gate.
      if (vehicle) {
        openGate();
        state = OPEN;
        break;
      }

      if (millis() - closingStart >= CLOSE_TIME) {
        closeGate();
        state = WAITING;
      }

      break;
  }
}
