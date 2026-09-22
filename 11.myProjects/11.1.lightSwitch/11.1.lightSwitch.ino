/*
  Author:
  Learning Intention: Students will use a range of simple sensors to control an LED UI

  Student Notes:
  1. Make sure you include a copy of your schematic
  2. Students should use either the button or potentiometer in an unconventional way

  3. Suggested Grove ports: Button D4, Potentiometer A0, LED D6, Light sensor A3
*/

const int BUTTON_PIN = 4;
const int LED_PIN = 6;

const unsigned long HOLD_TIME = 2000;

bool ledState = false;
bool timingHold = false;
bool actionDone = false;

unsigned long holdStart = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  bool pressed = digitalRead(BUTTON_PIN) == HIGH;

  if (pressed) {

    if (!timingHold) {
      timingHold = true;
      holdStart = millis();
      actionDone = false;
    }

    if (!actionDone && millis() - holdStart >= HOLD_TIME) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);

      actionDone = true;
    }

  } else {
    timingHold = false;
    actionDone = false;
  }
}
