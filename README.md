# **Laser Communication System**

> **A comprehensive project demonstrating wireless data transmission using laser technology, designed for secure and efficient communication applications.**

![License](https://img.shields.io/badge/License-MIT-blue) 
![Platform](https://img.shields.io/badge/Platform-Arduino-green) 
![Modulation](https://img.shields.io/badge/Modulation-NRZ-lightgrey)

---

## **✨ Features**
- 🚀 **Data Mapping and Encoding**: Converts text to binary with start and end flags for integrity.
- 🔦 **OOK Modulation**: Employs On-Off Keying to transmit binary data as laser pulses.
- 📡 **Real-Time Demodulation**: Reconstructs data from received laser signals.
- 🛡 **Error Handling**: Validates transmitted frames using flag-based framing.
- 💡 **Visual Indicators**: Uses LEDs for system state feedback.

---

## **📽 Demo**
> **Coming Soon!**

---

## **📥 Installation**

Clone this repository and set up the hardware and software:

```bash
git clone https://github.com/void0x11/Laser-Communication-System.git
```

### **Hardware Setup**
1. Assemble components based on the schematic in the `docs` folder.
2. Align transmitter and receiver for line-of-sight communication.

### **Software Setup**
1. Flash the code from `LaserCommunication.cpp` and `LaserCommunication.h` to the microcontroller using Arduino IDE.
2. Configure transmitter and receiver roles using `PointA.ino` and `PointB.ino` sketches.

---

## **📂 Project Structure**
```bash
Laser-Communication-System/
├── LaserCommunication.cpp   # Core implementation
├── LaserCommunication.h     # Header file
├── PointA.ino               # Transmitter setup
├── PointB.ino               # Receiver setup
├── docs/                    # Schematic and documentation
├── LICENSE                  # License file
└── README.md                # Project documentation
```

---

## **📜 License**

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code with proper attribution.

---

For questions or contributions, open an issue on the [GitHub repository](https://github.com/void0x11/Laser-Communication-System/issues).
