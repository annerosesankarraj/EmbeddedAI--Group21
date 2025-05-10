# EmbeddedAI--Group21
Assistive Guidance for Visually Impaired People Using Swarm Robots
=================================================================

Team 21
• Barath Kumar Jayachandran (bj2519)  
• Anne Rose Sankar Raj (as7525)  
• Yu Tz-Jie (ty2534)  

1. Project Overview
-------------------
Vision:
-------
To build a multi-robot indoor guidance system for visually impaired users.  
A drone builds a global map. Ground robots use that map for local navigation and obstacle avoidance.  
The system runs on low-power ESP32 boards and uses on-device AI for object detection.

Current Work:
-------------
We’ve built two ground robots that perform obstacle avoidance indoors using HC-SR04 ultrasonic sensors. Separately, we run object-detection inference on a Raspberry Pi 4 B with an ONNX model.  
Mapping by a drone, audio output to the user, and coordination among all three agents remain future work.

2. Hardware Requirements
------------------------
• ESP32 Dev Boards × 2  
• HC-SR04 ultrasonic sensors (one per robot)  
• L298N motor drivers + N20 DC motors  
• Raspberry Pi 4 B + camera module  
• Battery packs + chassis  

3. Software Requirements
------------------------
• Arduino IDE (for ESP32 firmware)  
• Python 3.8+  
• ONNX Runtime on Raspberry Pi  
• Edge Impulse CLI (for model export)  

4. Installation & Setup
-----------------------
1. Clone the repo:  
   `git clone https://github.com/annerosesankarraj/EmbeddedAI--Group21.git`  
2. Flash ground-robot firmware:  
   – Open `Groundbot Code/Groundbot1Code.ino` in Arduino IDE for BOT1
   – Open `Groundbot Code/Groundbot2Code.ino` in Arduino IDE for BOT2
   – Select “ESP32 Dev Module” and upload to each board  
4. Set up Raspberry Pi:  
   – Install ONNX Runtime: `pip install onnxruntime`  
   – Place `mobilenet_v2_float.onnx` in `raspberry_pi`  
   – Run `detect_picamera2_stream.py`

5. Usage
--------
1. Power on each ESP32 robot; they begin scanning for obstacles and steering around them.  
2. On the Pi, start object detection:  
   `python3 detect.py`  
3. Monitor logs separately (no coordination yet).
