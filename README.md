# Embedded State Monitor

## Description
This project consists of two parts: an embedded system program (LED_state_handler.c) and a monitoring application (application_monitor.py). The goal is to create an application on a personal computer that communicates with an MSP432 microcontroller via a serial port over a USB connection.

## Demo Video
Watch this demo recorded by Yash Kapoor for a detailed walkthrough of this project:
https://drive.google.com/file/d/1LSEILRXVEywjj1cTgE-NbKXY45yBeI8C/view?usp=sharing

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
3. Install the required Python libraries by typing the following commands in the command prompt:

Pyserial:
```
pip install pyserial
```
Keyboard:
```
pip install keyboard
```

## Usage
**Step 1**: Clone the repository by typing the following command in your terminal:
```
git clone https://github.com/YashKapoor1102/EmbeddedStateMonitor.git
```
**Step 2**: Navigate to the project directory where you cloned the repository:
```
cd EmbeddedStateMonitor
```
**Step 3**: Open LED_state_handler.c in Keil uVision.  
**Step 4**: Click on **Project > Build Target** to compile the code.  
**Step 5**: Ensure the microcontroller is connected to your computer. Go to **Flash > Download** to upload the compiled code to your microcontroller.  
**Step 6**: Run the monitoring application by typing the following command in the terminal:
```
python application_monitor.py
```
Then, you can interact with the application by entering a state (press "s" and type 0 (both LEDs OFF), 1 (RGB LED ON), 2 (RED LED ON), or 3 (both LEDs OFF)). To exit the application, press "q".

The application will display the current state of the LEDs. It will update the displayed state whenever a button on the microcontroller is pressed or a state is entered through the application monitor. 

## Credits
- Yash Kapoor
