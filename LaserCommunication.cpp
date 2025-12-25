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
    String start = "10101010"; // START_FLAG_BYTE
    String end = "01010101";   // END_FLAG_BYTE
    
    // Calculate CRC-8 for the payload (without spaces)
    String cleanPayload = binaryData;
    cleanPayload.replace(" ", "");
    uint8_t crc = calculateCRC8(cleanPayload);
    
    // Convert CRC to 8-bit binary string
    String crcStr = "";
    for (int i = 7; i >= 0; i--) {
        crcStr += (crc & (1 << i)) ? '1' : '0';
    }
    
    return start + " " + binaryData + " " + crcStr + " " + end;
}

String LaserCommunication::BinaryDataSeparation(String binary) {
    return binary;
}

String LaserCommunication::RemoveFlags(String binaryData) {
    String start = "10101010";
    String end = "01010101";
    
    int startIndex = binaryData.indexOf(start);
    int endIndex = binaryData.lastIndexOf(end);
    
    if (startIndex != -1 && endIndex != -1 && endIndex > startIndex + 16) {
        // Extract payload and CRC
        // Format: [START] [DATA...] [CRC] [END]
        // Remove spaces for easier indexing if needed, but current format has spaces.
        // Let's find the last space before 'end' to get CRC
        int lastSpace = binaryData.lastIndexOf(' ', endIndex - 1);
        String crcStr = binaryData.substring(lastSpace + 1, endIndex - 1);
        String payload = binaryData.substring(startIndex + 9, lastSpace);
        
        // Verify CRC
        String cleanPayload = payload;
        cleanPayload.replace(" ", "");
        uint8_t calcCrc = calculateCRC8(cleanPayload);
        
        uint8_t recvCrc = (uint8_t)strtol(crcStr.c_str(), NULL, 2);
        
        if (calcCrc == recvCrc) {
            Serial.println(F("[CRC] Verification Passed."));
            return payload;
        } else {
            Serial.print(F("[CRC] Verification Failed! Computed: "));
            Serial.print(calcCrc, HEX);
            Serial.print(F(", Received: "));
            Serial.println(recvCrc, HEX);
            return "Invalid data";
        }
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

String LaserCommunication::receiveOOK(int receiverPin, int bitDuration, int silenceThreshold, int led, int threshold) {
    String receivedData = "";
    unsigned long lastSignalTime = millis();
    bool receiving = false;

    while (millis() - lastSignalTime < silenceThreshold) {
        // Use analogRead if we want to support adaptive, but the pin might be digital-only info
        // Let's assume the user can pass a threshold or use digitalRead if it's a digital pin.
        // For simplicity, we'll check if threshold is -1 to use digitalRead.
        int signal;
        if (threshold == -1) {
            signal = digitalRead(receiverPin);
        } else {
            signal = (analogRead(receiverPin) > threshold) ? HIGH : LOW;
        }

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
        return receivedData;
    }
    return "";
}

uint8_t LaserCommunication::calculateCRC8(String binaryData) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < binaryData.length(); i += 8) {
        String byteStr = binaryData.substring(i, i + 8);
        uint8_t b = (uint8_t)strtol(byteStr.c_str(), NULL, 2);
        
        crc ^= b;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07; // Polynomial x^8 + x^2 + x^1 + 1
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

int LaserCommunication::getAdaptiveThreshold(int analogPin) {
    Serial.println(F("[CALIBRATION] Starting Adaptive Thresholding..."));
    long sum = 0;
    for (int i = 0; i < 50; i++) {
        sum += analogRead(analogPin);
        delay(10);
    }
    int ambient = sum / 50;
    int threshold = ambient + 50; // Add fixed offset (could be dynamic)
    Serial.print(F("[CALIBRATION] Ambient: "));
    Serial.print(ambient);
    Serial.print(F(", Set Threshold: "));
    Serial.println(threshold);
    return threshold;
}
