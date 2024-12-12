# Laser Communication System

A comprehensive **Laser Communication System** designed to demonstrate the transmission of data wirelessly using laser technology. This project leverages modulation techniques and robust data encoding methods to ensure reliable and secure communication. The system includes both a transmitter and receiver, utilizing On-Off Keying (OOK) modulation.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation and Usage](#installation-and-usage)
- [Project Highlights](#project-highlights)
- [Contributing](#contributing)
- [License](#license)

## Introduction

Laser communication uses light to transmit information over a line-of-sight path, offering an alternative to traditional wired and radio communication systems. This project demonstrates basic principles of laser-based data transmission, showcasing its efficiency, security, and practical implementation challenges.

## Features

- **Data Mapping and Encoding**: Converts text data to binary format with added start and end flags for transmission integrity.
- **Modulation Using OOK**: Employs On-Off Keying modulation to transmit binary data as laser pulses.
- **Real-Time Demodulation**: Processes received signals to reconstruct the original data.
- **Error Handling**: Ensures valid data frames with flag-based framing.
- **Visual Indicators**: Uses LEDs to signal system states during data transmission and reception.

## System Architecture

The system comprises the following components:

1. **Transmitter**: Encodes and modulates input data for transmission using a laser diode.
2. **Receiver**: Demodulates and decodes the received laser signals back into readable data.

### Block Diagram
```
[Data Source] --> [Transmitter (Laser Diode + Driver)] --> [Line-of-Sight Path] --> [Receiver (Photodiode + Amplifier)] --> [Output Device]
```

## Hardware Requirements

- Laser Diode Module
- Photodiode Sensor
- Signal Modulation and Demodulation Circuit
- LED Indicators
- Power Supply (e.g., 5V DC)
- Breadboard or PCB for prototyping

## Software Requirements

- Arduino IDE (or any compatible microcontroller programming environment)
- Simulation Tools (e.g., Proteus or LTspice for circuit validation)

## Installation and Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/void0x11/Laser-Communication-System.git
   ```

2. Assemble the hardware components based on the provided schematic in the `docs` folder.

3. Flash the provided code (`LaserCommunication.cpp` and `LaserCommunication.h`) onto the microcontroller using the Arduino IDE.

4. Align the transmitter and receiver components for a clear line-of-sight path.

5. Use the `PointA.ino` and `PointB.ino` sketches for configuring the transmitter and receiver roles.

6. Test the system by transmitting and receiving sample data, and monitor the Serial output for debugging information.

## Project Highlights

- **Encoding and Decoding**: Implements ASCII-to-binary conversion and vice versa with framing.
- **OOK Modulation**: Transmits binary data with visual feedback on system state.
- **Error Handling**: Validates transmitted data using start and end flags.
- **Practical Challenges**: Overcomes alignment issues and noise interference.

## Contributing

Contributions are welcome! If you have ideas for improving the system, please:

1. Fork the repository.
2. Create a new branch.
3. Commit your changes.
4. Open a pull request with detailed information about your updates.

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code with proper attribution.

---

For any questions or feedback, please open an issue on the [GitHub repository](https://github.com/void0x11/Laser-Communication-System/issues).
