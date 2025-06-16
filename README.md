# arduino-imu-demo
# IMU Temperature Correction Module

A lightweight Python module for correcting IMU (Inertial Measurement Unit) sensor readings based on temperature variations.  
Designed for use in embedded systems, robotics, or even space-oriented applications where temperature drift significantly affects sensor accuracy.

## 📌 Features

- Calibrates accelerometer and gyroscope readings using temperature data
- Supports real-time correction or batch post-processing
- Includes sample datasets and visualization scripts
- Works with Arduino-based IMU setups
- Lightweight and dependency-minimal

## 🛰 Use Case

This module was originally developed as part of a research project exploring the feasibility of using low-cost IMU sensors in high-drift or extreme environments — such as **space**.

You can read the detailed series of technical articles here (in Japanese):  
📖 [Zenn article series (全7回)](https://zenn.dev/yourusername/articles/imu-temp-correction-series)

## ⚙️ Installation

```bash
git clone https://github.com/twenty-eight2828/imu-temp-correction.git
cd imu-temp-correction
pip install -r requirements.txt
