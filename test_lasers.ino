#include "LaserCommunication.h"
LaserCommunication laser;

// Point A

// Channel AB Receiver
#define Rec_AB1 7
#define Rec_AB2 8

// Channel BA Sender
#define laser_BA1 2
#define laser_BA2 3

// LEDs represented
#define LedS_AB1 13 // LED Green for Sender BA1
#define LedR_BA1 22 // LED Red for Receiver AB1
#define LedS_AB2 24 // LED Green for Sender BA2
#define LedR_BA2 26 // LED Red for Receiver AB2

// Flicker timing in milliseconds
#define FLICKER_INTERVAL 50

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize laser pins
  pinMode(laser_BA1, OUTPUT);
  pinMode(laser_BA2, OUTPUT);

  // Initialize receiver pins
  pinMode(Rec_AB1, INPUT);
  pinMode(Rec_AB2, INPUT);

  // Initialize LED pins
  pinMode(LedS_AB1, OUTPUT);
  pinMode(LedR_BA1, OUTPUT);
  pinMode(LedS_AB2, OUTPUT);
  pinMode(LedR_BA2, OUTPUT);

  // Ensure all LEDs and lasers start OFF
  digitalWrite(laser_BA1, LOW);
  digitalWrite(laser_BA2, LOW);
  digitalWrite(LedS_AB1, LOW);
  digitalWrite(LedR_BA1, LOW);
  digitalWrite(LedS_AB2, LOW);
  digitalWrite(LedR_BA2, LOW);

  Serial.println("System Initialized");
}

void loop() {
  static unsigned long lastFlickerTime = 0;
  static bool laserState = LOW;

  unsigned long currentTime = millis();

  // Flicker the lasers continuously
  if (currentTime - lastFlickerTime >= FLICKER_INTERVAL) {
    lastFlickerTime = currentTime;
    laserState = !laserState;

    // Toggle lasers
    digitalWrite(laser_BA1, laserState);
    digitalWrite(laser_BA2, laserState);

    // Indicate laser state in the Serial Monitor
    Serial.println(laserState ? "Lasers ON" : "Lasers OFF");
  }

  // Check receivers and control red LEDs
  // Receiver AB1
  if (digitalRead(Rec_AB1) == LOW) { // Signal detected
    digitalWrite(LedR_BA1, HIGH);    // Turn Red LED ON
    Serial.println("Signal received on AB1");
  } else {
    digitalWrite(LedR_BA1, LOW);     // Turn Red LED OFF
  }

  // Receiver AB2
  if (digitalRead(Rec_AB2) == LOW) { // Signal detected
    digitalWrite(LedR_BA2, HIGH);    // Turn Red LED ON
    Serial.println("Signal received on AB2");
  } else {
    digitalWrite(LedR_BA2, LOW);     // Turn Red LED OFF
  }

  // Simulate sending data by toggling green LEDs during laser flicker
  digitalWrite(LedS_AB1, laserState); // Green LED for BA1 sender
  digitalWrite(LedS_AB2, laserState); // Green LED for BA2 sender
}
