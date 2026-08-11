/*
  Author:

  Learning Intention:
  The students will read a Grove Button module and control a Grove LED module,
  applying binary selection logic (if, if else, if else if else, switch/case).

  Success Criteria:
    1.  I can connect Grove modules to the base shield and know which port each uses
    2.  I know how to read binary data from a specific PIN (digitalRead)
    3.  I know how to write binary data to a specific PIN (digitalWrite)
    4.  I understand that LOW = false = 0 and HIGH = true = 1
    5.  I understand binary selection is asking questions with only true and false outcomes
    6.  I can apply comparison operators in if, if else, and if else if else structures
    7.  I understand a switch/case structure and the role of break
    8.  I understand the difference between a nested if and boolean operators
    9.  I can read a simple logic flowchart
    10. I can apply this knowledge to the Button and the Line Finder modules

  Hardware (Grove cables to the base shield):
    Button module  -> D4
    LED module     -> D6
    Line Finder    -> D7  (application task)

  Student Notes:
    Comparison Operators: != , < , <= , == , > , >=
    Boolean Operators: ! (not), && (and), || (or)
    The Grove Button reads HIGH while pressed. Inside the module a resistor
    holds the pin LOW when released. See the flowcharts in this folder's Readme.md.

  Optional breadboard reference (how this wiring works without Grove modules):
    Bootcamp-digitalWrite.png and Bootcamp-digitalRead.png in this folder.

  Documentation:
    https://sensorkit.arduino.cc/
    https://www.arduino.cc/reference/en/#structure
    https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
*/

#include "Ultrasonic.h"

const int BUZZER_PIN = 5;
const int LED_PIN = 6;

Ultrasonic ultrasonic(2);

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

int readDistance() {
  return ultrasonic.read();
}

int classifyZone(int distance, int nearLimit, int farLimit) {
  if (distance < nearLimit) {
    return 0;
  } else if (distance < farLimit) {
    return 1;
  }

  return 2;
}

void showAlert(int zone) {
  if (zone == 2) {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
  else if (zone == 1) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
    delay(200);
    noTone(BUZZER_PIN);
    delay(500);
  }
  else {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1500);
    delay(100);
    noTone(BUZZER_PIN);
    delay(100);
  }
}

void logStatus(int distance, int zone) {
  Serial.print(distance);
  Serial.print(" cm, zone ");
  Serial.println(zone);
}

void loop() {
  int distance = readDistance();
  int zone = classifyZone(distance, 10, 30);
  showAlert(zone);
  logStatus(distance, zone);
}
