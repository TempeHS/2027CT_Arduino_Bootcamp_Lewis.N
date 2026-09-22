/*
  Author:
  Learning Intention: Students will build a monitoring system that watches
  conditions and raises alerts, like the guardian inside a smart fridge

  The build: FRIDGE MONITOR
  1. Calibrate the light sensor FIRST (see below), then set your threshold
  2. The light sensor detects the door opening (light floods into a dark fridge)
  3. If the door stays open longer than 10 seconds, sound the buzzer and flash
     the LED until the door closes
  4. Keep the loop non-blocking: use the millis() pattern for the alarm flash
     AND the Serial Plotter output (no delay() anywhere)
  5. Plot the light level and door state so you can watch your threshold work

  Calibration (why: every bench is different):
    Ambient light, sensor tolerance and how enclosed your "fridge" is all
    change the raw readings, so a threshold that works for one build fails on
    another. Upload a bare reader first:
        void setup() { Serial.begin(115200); }
        void loop()  { Serial.println(analogRead(A3)); delay(50); }
    Watch the Serial Plotter. Record the covered (door closed) and uncovered
    (door open) values, set your threshold roughly halfway between them, and
    keep both measurements as a comment: evidence the number is not a guess.

  Student Notes:
  1. Make sure you include a flowchart of your logic
  2. Stretch goal: add a second sensor (Temp & Humidity I2C) or an OLED dashboard

  Suggested Grove ports: Light A3, Buzzer D5, LED D6
*/

const int LIGHT_PIN = A3;
const int BUZZER_PIN = 5;
const int LED_PIN = 6;

// Replace these with YOUR calibration measurements
// Covered: 150
// Uncovered: 750
const int DOOR_OPEN_THRESHOLD = 450;

const unsigned long OPEN_TIME = 10000;
const unsigned long FLASH_INTERVAL = 500;

unsigned long doorOpenStart = 0;
unsigned long lastFlash = 0;

bool doorTimerStarted = false;
bool alarmState = false;

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  unsigned long currentTime = millis();

  int lightLevel = analogRead(LIGHT_PIN);

  bool doorOpen = lightLevel > DOOR_OPEN_THRESHOLD;

  // Serial Plotter
  Serial.print("Light:");
  Serial.print(lightLevel);
  Serial.print("\tDoor:");
  Serial.println(doorOpen ? 1 : 0);

  if (doorOpen) {

    if (!doorTimerStarted) {
      doorOpenStart = currentTime;
      doorTimerStarted = true;
    }

    if (currentTime - doorOpenStart >= OPEN_TIME) {

      if (currentTime - lastFlash >= FLASH_INTERVAL) {
        lastFlash = currentTime;

        alarmState = !alarmState;

        digitalWrite(LED_PIN, alarmState ? HIGH : LOW);
        digitalWrite(BUZZER_PIN, alarmState ? HIGH : LOW);
      }
    }

  } else {

    // Door closed
    doorTimerStarted = false;
    alarmState = false;

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}
