<h1 align="center">⚖️ Smart AI Weighting & Billing System</h1>

<p align="center">
  <b>An embedded TinyML system that identifies items visually, measures weight, and calculates price in real time using ESP32-CAM, HX711, and Edge Impulse.</b>
</p>

<p align="center">
  Built for smart retail and automated checkout scenarios, this project combines computer vision, sensor data, and on-device inference in a fast, low-cost, fully offline solution.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/ESP32--CAM-Vision%20Node-black?style=for-the-badge&logo=espressif&logoColor=white" />
  <img src="https://img.shields.io/badge/Edge%20Impulse-TinyML-6E56CF?style=for-the-badge" />
  <img src="https://img.shields.io/badge/HX711-Load%20Cell-1E88E5?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Arduino-Embedded%20C%2B%2B-00979D?style=for-the-badge&logo=arduino&logoColor=white" />
</p>

<p align="center">
  <img src="https://skillicons.dev/icons?i=cpp,arduino" />
</p>

---

## 🚀 Overview

The **Smart AI Weighing & Billing System** is an embedded AI project that automates product recognition and billing in a compact edge device.

It uses an **ESP32-CAM** to capture product images, a **load cell with HX711** to measure weight, and an **Edge Impulse TinyML model** to classify the detected item locally. Based on the predicted item class and measured weight, the system calculates the total cost instantly without relying on the cloud.

---

## ✨ Features

- Automatic visual item detection
- Weight-based product measurement
- Real-time billing calculation
- On-device machine learning inference
- Fully offline operation
- Confidence-based item classification
- Low-cost and scalable hardware design
- Suitable for smart retail and automated checkout systems

---

## 🧠 How It Works

1. The **ESP32-CAM** captures a 96×96 grayscale image of the product.
2. The system checks whether a valid weight is placed on the scale.
3. The image is passed to an **Edge Impulse** model running on-device.
4. The model classifies the item, such as onion or potato.
5. The **HX711** reads the load cell value and calculates the item weight.
6. The system multiplies the measured weight by the predefined price of the detected item.
7. The final bill is generated in real time.

---

## 🛠️ Tech Stack

### Hardware
- ESP32-CAM (AI Thinker)
- HX711 load cell amplifier
- Load cell sensor
- Power supply module
- Supporting jumper wires and prototype setup

### Software
- Arduino IDE
- Embedded C++
- Edge Impulse
- TinyML inference pipeline

### Concepts Used
- Embedded systems
- Computer vision
- Sensor fusion
- Edge AI
- Real-time automation

---

## 🔩 Hardware Components

| Component | Purpose |
|---|---|
| ESP32-CAM | Captures images and runs the embedded application |
| OV2640 Camera | Provides image input for item classification |
| HX711 | Amplifies and digitizes load cell signals |
| Load Cell | Measures object weight |
| Edge Impulse Model | Performs on-device item classification |

---

## 📷 Image Processing

The vision pipeline uses low-resolution grayscale image capture to reduce memory usage and keep inference lightweight for embedded deployment.

This makes the system faster and more practical for microcontroller-based AI applications where memory and processing power are limited.

---

## ⚖️ Weight Measurement

The load cell measures the physical weight of the placed item, while the **HX711** converts the analog sensor output into a digital value that the microcontroller can process.

This allows the system to combine product identity and measured mass to generate billing data automatically.

---

## 💰 Billing Logic

The billing logic follows a simple rule:

```text
Total Price = Item Weight × Price Per Unit
```

Example:

```text
Detected Item: Onion
Measured Weight: 1.50 kg
Price Per Kg: 40
Total Bill: 60
```

Each product class can be mapped to its own unit price inside the firmware.

---

## 🎯 Use Cases

- Smart grocery stores
- Automated checkout counters
- Self-service weighing systems
- Small retail automation
- Educational TinyML demonstrations
- Embedded AI product prototypes

---

## 📁 Project Structure

```bash
smart-ai-weighing-billing/
├── src/
│   ├── main.ino
│   ├── camera_handler.h
│   ├── camera_handler.cpp
│   ├── weight_sensor.h
│   ├── weight_sensor.cpp
│   ├── billing_logic.h
│   ├── billing_logic.cpp
│   └── model_inference.h
├── model/
│   ├── edge-impulse-model/
├── docs/
│   ├── circuit-diagram.png
│   ├── system-architecture.png
│   └── demo-images/
├── README.md
```

---

## ⚙️ Setup

### 1. Clone the repository

```bash
git clone https://github.com/your-username/smart-ai-weighing-billing.git
cd smart-ai-weighing-billing
```

### 2. Open the project in Arduino IDE

Make sure the ESP32 board package is installed and the correct board is selected.

### 3. Connect hardware

- Connect the load cell to the HX711
- Connect HX711 to the ESP32-CAM GPIO pins
- Connect the camera module properly
- Power the board using a stable supply

### 4. Add the Edge Impulse model files

Export your trained model from Edge Impulse and place the generated inference files inside the project.

### 5. Upload the code

Compile and flash the firmware to the ESP32-CAM.

---

## 🧪 Workflow Summary

- Capture image from camera
- Preprocess image
- Run TinyML inference
- Read load cell value
- Determine item type
- Compute billing amount
- Output result in real time

---

## 📈 Future Improvements

- Add LCD or OLED display for live output
- Add thermal receipt printing
- Support more product classes
- Improve dataset quality and model accuracy
- Add wireless dashboard monitoring
- Save transaction history
- Add barcode or QR fallback support
- Integrate with inventory systems

---

## 🎓 What I Learned

This project gave me hands-on experience in:

- TinyML model deployment on edge hardware
- Embedded sensor integration
- Load cell calibration and signal acquisition
- Camera-based classification on constrained devices
- Combining vision and weight data for automation
- Designing low-cost AI systems for real-world use cases

---

## 🌍 Applications

This project demonstrates how low-cost embedded hardware can be used to create intelligent retail systems without cloud dependency.

It is especially useful in scenarios where fast response, offline capability, and affordability are important.

---



## ⭐ Support

If you found this project interesting, consider giving it a **star** on GitHub.

---
