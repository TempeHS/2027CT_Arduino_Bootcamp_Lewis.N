// Move your helper functions into this tab.
// The Arduino build system automatically joins every .ino file in this folder
// into one program, so functions defined here can be called from the main sketch.

int readDistance() {
  return ultrasonic.read();
}

int classifyZone(int distance, int nearLimit, int farLimit) {
  if (distance < nearLimit) {
    return 0;              // danger
  } else if (distance < farLimit) {
    return 1;              // warning
  }
  return 2;                // safe
}

void showAlert(int zone) {
  if (zone == 2) {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
  else if (zone == 1) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 262);
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);
  }
  else {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 523);
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
