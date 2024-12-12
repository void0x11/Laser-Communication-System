#include <LaserCommunication.h>

// Main Function: Maps data between text and binary
String LaserCommunication::TextMapData(String data, int mode) {
    if (mode == 1) {
        // Mode 1: Sender
        String asciiData = AsciiDataSeparation(data);        // Separate ASCII data
        String binaryData = Text2Binary(asciiData);          // Convert to binary
        return AddFlags(binaryData);                         // Add flags and return mapped data
    } else if (mode == 2) {
        // Mode 2: Receiver
        String binaryData = BinaryDataSeparation(data);      // Separate binary data
        String cleanedData = RemoveFlags(binaryData);        // Remove flags
        return Binary2Text(cleanedData);                    // Convert binary to text
    } else {
        return "Invalid mode"; // Handle invalid mode
    }
}

// Separates a string into an array of characters
String LaserCommunication::AsciiDataSeparation(String text) {
    return text; // Directly return the string; Arduino `String` already supports indexing
}

// Converts text characters to binary
String LaserCommunication::Text2Binary(String asciiData) {
    String binaryString = "";
    for (size_t i = 0; i < asciiData.length(); i++) {
        char c = asciiData[i];
        String binaryChar = String(c, BIN); // Convert character to binary
        while (binaryChar.length() < 8) {
            binaryChar = "0" + binaryChar;   // Ensure 8-bit representation
        }
        binaryString += binaryChar + " ";  // Add space between binary values
    }
    binaryString.trim(); // Remove trailing space
    return binaryString;
}

// Adds start and end flags to the binary data
String LaserCommunication::AddFlags(String binaryData) {
    return String(START_FLAG) + " " + binaryData + " " + END_FLAG; // Add start and end flags
}

// Separates binary data for processing
String LaserCommunication::BinaryDataSeparation(String binary) {
    return binary; // Return the binary data as-is; assume input is valid binary
}

// Removes the start and end flags from binary data
String LaserCommunication::RemoveFlags(String binaryData) {
    int startIndex = binaryData.indexOf(START_FLAG) + String(START_FLAG).length() + 1;
    int endIndex = binaryData.lastIndexOf(END_FLAG) - 1;
    if (startIndex >= 0 && endIndex > startIndex) {
        return binaryData.substring(startIndex, endIndex);
    }
    return "Invalid data"; // Handle cases where flags are missing or malformed
}

// Converts binary data back to text
String LaserCommunication::Binary2Text(String binaryData) {
    String text = "";
    for (size_t i = 0; i < binaryData.length(); i += 9) { // Each 8 bits + space
        String binaryChar = binaryData.substring(i, i + 8);
        char c = (char)strtol(binaryChar.c_str(), NULL, 2); // Convert binary to ASCII
        text += c;
    }
    return text;
}


// Function to modulate binary data using OOK
void LaserCommunication::sendOOK(String binaryData, int laserPin, int bitDuration, int led) {
  Serial.println("Starting OOK modulation...");
  Serial.print("Binary Data: ");
  Serial.println(binaryData);

  for (int i = 0; i < binaryData.length(); i++) {
    char bit = binaryData[i];
    if (bit == '1') {
      digitalWrite(laserPin, HIGH);  // Laser ON
      Serial.println("Laser ON for '1'");
    } else if (bit == '0') {
      digitalWrite(laserPin, LOW);   // Laser OFF
      Serial.println("Laser OFF for '0'");
    }
	digitalWrite(led,HIGH);
    delay(bitDuration);  // Hold state for bit duration
  }
  digitalWrite(laserPin, LOW);  // Turn OFF laser after transmission
  Serial.println("OOK modulation complete.");
  digitalWrite(led,LOW);
}


String LaserCommunication::receiveOOK(int receiverPin, int bitDuration, int silenceThreshold, int led) {
  String receivedData = "";  // To store the binary data frame
  unsigned long lastSignalTime = millis(); // Track the last time a signal was detected
  bool receiving = false; // Flag to track if we are receiving data

  Serial.println("Starting OOK demodulation...");

  // Loop until timeout or receiving completes
  while (millis() - lastSignalTime < silenceThreshold) {
    int signal = digitalRead(receiverPin); // Read the state of the receiver

    // Detect and append bits based on signal
    if (signal == HIGH) {
      receiving = true;  // We are receiving data
      lastSignalTime = millis(); // Reset the timeout

      // Append '1' since signal is detected
      receivedData += '1';
      digitalWrite(led, LOW); // Indicate receiving with LED
    } else if (receiving) {
      // Append '0' when no signal is detected
      receivedData += '0';
    }

    delay(bitDuration); // Wait for the next bit duration
  }

  if (receiving) {
    Serial.println("OOK demodulation complete.");
    Serial.print("Raw Received Data Frame: ");
    Serial.println(receivedData);
	digitalWrite(led, HIGH);
    // Detect valid start and end flags
    String startFlag = "11111111";
    String endFlag = "11111111";

    int startFlagIndex = receivedData.indexOf(startFlag);
    int endFlagIndex = receivedData.lastIndexOf(endFlag);

    // Ensure start and end flags are valid and properly positioned
    if (startFlagIndex != -1 && endFlagIndex != -1 && startFlagIndex < endFlagIndex) {
      // Extract the data between the flags
      String validData = receivedData.substring(startFlagIndex + 8, endFlagIndex);

      if (validData.length() > 0) {
        Serial.println("Valid Data Found:");
        Serial.println(validData);
        return validData; // Return the valid data frame
      } else {
        Serial.println("No valid data found between flags.");
      }
    } else {
      Serial.println("Start or end flag not detected or malformed data.");
    }
  } else {
    Serial.println("No data received (timeout).");
  }
	digitalWrite(led, LOW);
  return ""; // Return an empty string if no valid data is found
}