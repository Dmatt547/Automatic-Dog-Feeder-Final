Automatic Dog Feeder

An IoT-based automatic dog feeder that monitors food levels and dispenses food based on weight thresholds, ensuring pets are fed the correct amount even when the owner is away.

Overview

This system uses a load cell sensor to detect how much food is in the bowl and automatically dispenses more when it drops below a set threshold. It also tracks temperature and humidity and displays all data in real time.

Demo: https://youtu.be/B0yXcGgZ2DM

Full artefact:

Features
Weight-based automatic feeding
Temperature & humidity monitoring
Real-time data display (Tkinter GUI)
Remote monitoring via Raspberry Pi
System
Arduino Nano 33 IoT → collects sensor data
Raspberry Pi → processes data + displays GUI
Sensors → Load cell (weight), DHT11 (temp/humidity)
Servo motor → controls food dispensing

Data is sent from the Arduino to the Raspberry Pi via serial communication for real-time display.

How It Works
Measure food weight
If below threshold → activate feeder
Servo dispenses food
Data updates live on GUI
Challenges
Load cell accuracy and noise
Inconsistent food flow from funnel
Arduino ↔ Raspberry Pi communication
Improvements
More accurate dispensing mechanism (e.g. tilting system)
Wireless communication
Better precision and reliability
Tech Stack

Arduino (C/C++), Python (Tkinter), Raspberry Pi, IoT sensors

Key Learnings
Hardware + software integration
Real-time data handling
Debugging embedded systems
