#include <LaserCommunication.h>

// Main Function: Maps data between text and binary
String LaserCommunication::TextMapData(String data, int mode) {
    if (mode == 1) {
        String asciiData = AsciiDataSeparation(data);
        String binaryData = Text2Binary(asciiData);
        return AddFlags(binaryData);
    } else if (mode == 2) {
        String cleanedData = RemoveFlags(data);
        if (cleanedData == "Invalid data") return "Invalid data";
        return Binary2Text(cleanedData);
    }
    return "Invalid mode";
}

String LaserCommunication::AsciiDataSeparation(String text) {
    return text;
}

String LaserCommunication::Text2Binary(String asciiData) {
    String binaryString = "";
    for (size_t i = 0; i < asciiData.length(); i++) {
        char c = asciiData[i];
        for (int b = 7; b >= 0; b--) {
            binaryString += (c & (1 << b)) ? '1' : '0';
        }
        binaryString += " ";
    }
    binaryString.trim();
    return binaryString;
}

String LaserCommunication::AddFlags(String binaryData) {
    String start = "10101010"; // Placeholder for START_FLAG_BYTE
    String end = "01010101";   // Placeholder for END_FLAG_BYTE
    return start + " " + binaryData + " " + end;
}

String LaserCommunication::BinaryDataSeparation(String binary) {
    return binary;
}

String LaserCommunication::RemoveFlags(String binaryData) {
    String start = "10101010";
    String end = "01010101";
    
    int startIndex = binaryData.indexOf(start);
    int endIndex = binaryData.lastIndexOf(end);
    
    if (startIndex != -1 && endIndex != -1 && endIndex > startIndex + 8) {
        return binaryData.substring(startIndex + 9, endIndex - 1);
    }
    return "Invalid data";
}

String LaserCommunication::Binary2Text(String binaryData) {
    String text = "";
    String currentByte = "";
    for (size_t i = 0; i < binaryData.length(); i++) {
        if (binaryData[i] == ' ') {
            if (currentByte.length() == 8) {
                text += (char)strtol(currentByte.c_str(), NULL, 2);
            }
            currentByte = "";
        } else {
            currentByte += binaryData[i];
        }
    }
    if (currentByte.length() == 8) {
        text += (char)strtol(currentByte.c_str(), NULL, 2);
    }
    return text;
}

void LaserCommunication::transmit(String binaryData, int laserPin, int bitDuration, int led, ModulationType type) {
    if (type == ModulationType::MANCHESTER) {
        sendManchester(binaryData, laserPin, bitDuration, led);
    } else {
        sendOOK(binaryData, laserPin, bitDuration, led);
    }
}

void LaserCommunication::sendOOK(String binaryData, int laserPin, int bitDuration, int led) {
    Serial.println(F("[SYSTEM] Starting OOK Modulation..."));
    for (size_t i = 0; i < binaryData.length(); i++) {
        char bit = binaryData[i];
        if (bit == ' ' ) continue;
        
        digitalWrite(laserPin, bit == '1' ? HIGH : LOW);
        digitalWrite(led, bit == '1' ? HIGH : LOW);
        delay(bitDuration);
    }
    digitalWrite(laserPin, LOW);
    digitalWrite(led, LOW);
    Serial.println(F("[SYSTEM] OOK Transmission Complete."));
}

void LaserCommunication::sendManchester(String binaryData, int laserPin, int bitDuration, int led) {
    Serial.println(F("[SYSTEM] Starting Manchester Encoding..."));
    for (size_t i = 0; i < binaryData.length(); i++) {
        char bit = binaryData[i];
        if (bit == ' ' ) continue;
        
        // Manchester: '1' -> HIGH-LOW, '0' -> LOW-HIGH
        if (bit == '1') {
            digitalWrite(laserPin, HIGH);
            digitalWrite(led, HIGH);
            delay(bitDuration / 2);
            digitalWrite(laserPin, LOW);
            digitalWrite(led, LOW);
            delay(bitDuration / 2);
        } else {
            digitalWrite(laserPin, LOW);
            digitalWrite(led, LOW);
            delay(bitDuration / 2);
            digitalWrite(laserPin, HIGH);
            digitalWrite(led, HIGH);
            delay(bitDuration / 2);
        }
    }
    digitalWrite(laserPin, LOW);
    digitalWrite(led, LOW);
    Serial.println(F("[SYSTEM] Manchester Transmission Complete."));
}

String LaserCommunication::receiveOOK(int receiverPin, int bitDuration, int silenceThreshold, int led) {
    String receivedData = "";
    unsigned long lastSignalTime = millis();
    bool receiving = false;

    while (millis() - lastSignalTime < silenceThreshold) {
        int signal = digitalRead(receiverPin);
        if (signal == HIGH) {
            receiving = true;
            lastSignalTime = millis();
            receivedData += '1';
        } else if (receiving) {
            receivedData += '0';
        }
        delay(bitDuration);
    }

    if (receiving) {
        // Simple logic for flag searching (to be improved)
        return receivedData;
    }
    return "";
}
