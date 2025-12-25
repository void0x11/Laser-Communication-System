#include "LaserCommunication.h"
LaserCommunication laser;

// Point B

// Channel AB Receiver
#define recv_AB1 3
#define recv_AB2 2

// Channel BA Sender
#define sens_BA1 10
#define sens_BA2 9

// LEDs represented
#define LedR_AB1 13 // LED Red for Sender BA1
#define LedS_BA1 7 // LED Green for Receiver AB1
#define LedR_AB2 30 // LED Red for Sender BA2
#define LedS_BA2 22 // LED Green for Receiver AB2
#define bitDuration 250
#define silenceThreshold 600

// Flicker timing in milliseconds
#define FLICKER_INTERVAL 50

String DataRt = ""; // Data sent
String DataFrame = "";

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);   // Serial connected to your PC
  
  // Initialize laser pins
  pinMode(sens_BA1, OUTPUT);
  pinMode(sens_BA2, OUTPUT);

  // Initialize receiver pins
  pinMode(recv_AB1, INPUT);
  pinMode(recv_AB2, INPUT);

  // Initialize LED pins
  pinMode(LedR_AB1, OUTPUT);
  pinMode(LedS_BA1, OUTPUT);
  pinMode(LedR_AB2, OUTPUT);
  pinMode(LedS_BA2, OUTPUT);

  // Ensure all LEDs and lasers start OFF
  digitalWrite(sens_BA1, LOW);
  digitalWrite(sens_BA2, LOW);
  digitalWrite(LedR_AB1, LOW);
  digitalWrite(LedS_BA1, LOW);
  digitalWrite(LedR_AB2, LOW);
  digitalWrite(LedS_BA2, LOW);
  Serial.println("System Initialized");
  Serial.println("System ready. Enter a message to send:");
}

ModulationType currentModulation = ModulationType::MANCHESTER; // Default to Manchester for better demo

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.equalsIgnoreCase("MODE:OOK")) {
      currentModulation = ModulationType::OOK;
      Serial.println(F("[CONFIG] Set modulation to OOK"));
      return;
    } else if (input.equalsIgnoreCase("MODE:MAN")) {
      currentModulation = ModulationType::MANCHESTER;
      Serial.println(F("[CONFIG] Set modulation to Manchester"));
      return;
    }

    if (input.length() > 0) {
      Serial.print(F("[SEND] Message: "));
      Serial.println(input);
      
      String frame = laser.TextMapData(input, 1);
      Serial.print(F("[SEND] Binary Frame: "));
      Serial.println(frame);
      
      laser.transmit(frame, sens_BA1, bitDuration, LedS_BA1, currentModulation);
      Serial.println(F("[SEND] Transmission complete."));
      Serial.println(F("-----------------------------------"));
    }
  }
}
