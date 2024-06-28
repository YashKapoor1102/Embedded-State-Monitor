# EmbeddedStateMonitor

## Description
This project consists of two parts: an embedded system program (LED_state_handler.c) and a monitoring application (application_monitor.py). The goal is to create a simple application on a personal computer that communicates with an MSP432 microcontroller via a serial port over a USB connection.

### Embedded System 
The embedded system program (LED_state_handler.c) manages the state of two LEDs on the microcontroller (an RGB LED and a Red LED). There are four states (00, 01, 10, 11) that the LEDs can be in, where each state represents a different combination of LED ON/OFF statuses. Button 1 (P1.1) cycles forward through the states and Button 2 (P1.4) cycles backward.

### Monitoring Application
The monitoring application is a Python script that communicates with the microcontroller to display and control the LED states. The user can input a state (0, 1, 2, or 3) to set the LEDs, and the application continuously displays the current state of the LEDs, updating whenever the state changes. 

## Installation
### Requirements
- MSP432 Microcontroller
- Python (Pyserial and Keyboard Library)
- IDE that supports the MSP432 Microcontroller (e.g., Keil uVision).

### Steps
1. Download Keil uVision from https://www.keil.com/demo/eval/arm.htm.
2. Download the latest version of Python from https://www.python.org/downloads/.
3. Install the required Python libraries:

Pyserial:
```
pip install pyserial
```
Keyboard:
```
pip install keyboard
```
