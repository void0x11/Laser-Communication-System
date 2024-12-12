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

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize laser pins
  pinMode(sens_AB1, OUTPUT);
  pinMode(sens_AB2, OUTPUT);

  // Initialize receiver pins
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

  Serial.println("System Initialized");
}

void loop() {
  // Variables to store received data
  String receivedFrame = "";
  String decodedMessage = "";

  // Start receiving data using OOK
  receivedFrame = laser.receiveOOK(recv_BA1, bitDuration, silenceThreshold, LedR_BA1);

  if (receivedFrame.length() > 0) {
    Serial.print("Raw Received Frame: ");
    Serial.println(receivedFrame);

    // Decode the received data frame
    decodedMessage = laser.TextMapData(receivedFrame, 2);
    Serial.print("Decoded Message: ");
    Serial.println(decodedMessage);
  } else {
    Serial.println("No data received. Waiting...");
  }

  delay(500); // Small delay to prevent continuous polling
}
