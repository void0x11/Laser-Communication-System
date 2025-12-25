#include "LaserCommunication.h"
LaserCommunication laser;

#define bitDuration 250
#define silenceThreshold 600

// Point A

// Channel AB Receiver
#define recv_BA1 7
#define recv_BA2 8

// Channel BA Sender
#define sens_AB1 2
#define sens_AB2 3

// LEDs represented
#define LedR_BA1 22 // LED Red for Receiver AB1
#define LedR_BA2 26 // LED Red for Receiver AB2
#define LedS_AB1 13 // LED Green for Sender BA1
#define LedS_AB2 24 // LED Green for Sender BA2


// Flicker timing in milliseconds
#define FLICKER_INTERVAL 50

int threshold = -1; // Default to digital

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize laser pins
  pinMode(sens_AB1, OUTPUT);
  pinMode(sens_AB2, OUTPUT);

  // Initialize receiver pins
  // NOTE: For adaptive thresholding, connect receiver to an Analog pin (e.g., A0)
  pinMode(recv_BA1, INPUT);
  pinMode(recv_BA2, INPUT);

  // Initialize LED pins
  pinMode(LedR_BA1, OUTPUT);
  pinMode(LedR_BA2, OUTPUT);
  pinMode(LedS_AB1, OUTPUT);
  pinMode(LedS_AB2, OUTPUT);

  // Ensure all LEDs and lasers start OFF
  digitalWrite(sens_AB1, LOW);
  digitalWrite(sens_AB2, LOW);
  digitalWrite(LedR_BA1, LOW);
  digitalWrite(LedR_BA2, LOW);
  digitalWrite(LedS_AB1, LOW);
  digitalWrite(LedS_AB2, LOW);

  Serial.println(F("System Initialized"));
  
  // Optional: Calibrate adaptive threshold if using analog
  // threshold = laser.getAdaptiveThreshold(A0); 
}

void loop() {
  // Check for incoming data
  // Pass 'threshold' to use adaptive (if calibrated) or -1 for digital
  String receivedFrame = laser.receiveOOK(recv_BA1, bitDuration, silenceThreshold, LedR_BA1, threshold);

  if (receivedFrame.length() > 0) {
    Serial.println(F("\n[RECEIVE] Incoming transmission detected..."));
    Serial.print(F("[RECEIVE] Raw Frame: "));
    Serial.println(receivedFrame);

    String decodedMessage = laser.TextMapData(receivedFrame, 2);
    Serial.print(F("[RECEIVE] Decoded: "));
    Serial.println(decodedMessage);
    Serial.println(F("-----------------------------------"));
  }

  delay(100); // Responsive loop
}
