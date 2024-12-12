// LaserCommunication.h
#ifndef LASERCOMMUNICATION_H
#define LASERCOMMUNICATION_H

#include <Arduino.h>

#define START_FLAG "10101010"
#define END_FLAG "01010101"

class LaserCommunication {
public:
    // Main Function: Maps data between text and binary
    String TextMapData(String data, int mode);
    // Function to separate a string into ASCII characters
    String AsciiDataSeparation(String text);
    // Function to convert text characters to binary
    String Text2Binary(String asciiData);
    // Function to add start and end flags to binary data
    String AddFlags(String binaryData);
    // Function to separate binary data for processing
    String BinaryDataSeparation(String binary);
    // Function to remove start and end flags from binary data
    String RemoveFlags(String binaryData);
    // Function to convert binary data back to text
    String Binary2Text(String binaryData);
    // Function to modulate binary data using OOK
    void sendOOK(String binaryData, int laserPin, int bitDuration, int led);
    // Function to receive OOK modulated data
    String receiveOOK(int receiverPin, int bitDuration, int silenceThreshold, int led);
};
#endif