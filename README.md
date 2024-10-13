# ESP8266 WiFi Portable Box

## Overview
This project is a portable IoT box powered by an ESP8266 WiFi module, designed to display real-time data from a ThingSpeak channel on an LCD screen. The device is optimized for low power consumption and housed in a custom 3D-printed case.

## Features
- Displays live data via an LCD screen
- Battery-powered with deep sleep functionality
- Custom 3D-printed case
- Built-in button for power control

## Technology Stack
- **Hardware**: ESP8266, LCD display, button, battery pack
- **Software**: Arduino IDE, ThingSpeak API

## Setup Instructions
### Hardware Setup
1. Connect the ESP8266 to the LCD display and button as shown in the wiring diagram.
2. ...

### Software Setup
1. Install the necessary libraries in the Arduino IDE:
   - `ESP8266WiFi`
   - `ThingSpeak`
2. ...

### Usage
To start the device, press the button...

## Photos
![ESP Box Setup](images/esp_box_setup.jpg)

## Code Explanation
The main feature of this code is the power-saving deep sleep mode, which...

## Challenges & Learnings
I encountered some challenges with power management, and through this project, I learned about optimizing IoT devices for battery usage.

## Future Improvements
- Add more sensors to track additional data
- Improve the LCD display readability in sunlight
