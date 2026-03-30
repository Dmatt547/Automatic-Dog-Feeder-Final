# Automatic Dog Feeder

## Overview
An IoT-based automatic dog feeder that monitors food levels and dispenses food based on weight thresholds. This ensures pets are fed the correct amount even when the owner is away.

The system uses a **load cell sensor** to detect remaining food and automatically refills the bowl when needed. It also tracks **temperature and humidity** and displays all data in real time.

Demo: https://youtu.be/B0yXcGgZ2DM  
Full artefact: see project report

---

## Features
- Weight-based automatic feeding  
- Temperature & humidity monitoring  
- Real-time data display (Tkinter GUI)  
- Remote monitoring via Raspberry Pi  

---

## System Architecture

### Hardware
- Arduino Nano 33 IoT  
- Raspberry Pi  
- Load Cell Sensor  
- DHT11 Sensor  
- Servo Motor  

### Software
- Arduino (C/C++)  
- Python (Tkinter GUI)  

Data is sent from the Arduino to the Raspberry Pi via **serial communication** for real-time processing and display.

---

## How It Works
1. Measure food weight in the bowl  
2. If below threshold → activate feeder  
3. Servo dispenses food  
4. Data updates live on GUI  

---

## Challenges
- Load cell accuracy and noise  
- Inconsistent food flow from funnel  
- Arduino ↔ Raspberry Pi communication  

---

## Future Improvements
- More accurate dispensing mechanism (e.g. tilting system)  
- Wireless communication  
- Improved precision and reliability  

---

## Tech Stack
- Arduino (C/C++)  
- Python (Tkinter)  
- Raspberry Pi  
- IoT Sensors  

---

## Key Learnings
- Hardware + software integration  
- Real-time data handling  
- Debugging embedded systems  
