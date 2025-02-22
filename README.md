# 🚀 Embedded System: STM32 + BeagleBone Black + LoRa Communication

## 📌 Project Overview

This project integrates an **STM32 NUCLEO-F756ZG** microcontroller with a **BeagleBone Black (BBB)** using **LoRa (Long Range Radio Communication)** to **wirelessly transmit sensor data**. The system consists of **an outdoor STM32 unit** collecting sensor data and transmitting it via **LoRa**, while the **indoor BeagleBone Black unit** receives and processes the data.

## 🏷 System Architecture

The project is divided into **two main parts**:

### **1️⃣ Outdoor Unit: STM32F7**

- Powered by a **5V Power Bank**
- Equipped with:
  - ✅ **LoRa Module** (SPI Communication)
  - ✅ **HC-SR04 Ultrasonic Sensor** (Measures distance)
  - ✅ **SG90 Servo Motor** (Rotates ultrasonic sensor)
  - ✅ **OLED Display** (Shows sensor data)
  - ✅ **GPIO LED** (Status Indicator)
- **Functionality:**
  - The **STM32 reads sensor data** (Ultrasonic).
  - The **Servo smoothly rotates** from **0° to 180°** for scanning.
  - The **data is displayed on the OLED screen**.
  - The **processed data is sent wirelessly via LoRa** to the BBB.

### **2️⃣ Indoor Unit: BeagleBone Black**

- Acts as a **LoRa Receiver & Data Processor**
- Runs **Python scripts** to:
  - ✅ Receive LoRa messages via **SPI**
  - ✅ Process and store received data
  - ✅ Transmit data via **Ethernet** to a **Jetson Nano**
  - ✅ Serve data through a **C/C++ API**

## ⚡ Hardware & Wiring

| **Component**          | **STM32F7 Pin**   | **BeagleBone Black Pin** |
| ---------------------- | ----------------- | ------------------------ |
| **LoRa NSS**           | PA\_4             | P9.17 (SPI0\_CS0)        |
| **LoRa MOSI**          | PB\_5             | P9.18 (SPI0\_D1)         |
| **LoRa MISO**          | PA\_6             | P9.21 (SPI0\_D0)         |
| **LoRa SCK**           | PA\_5             | P9.22 (SPI0\_SCLK)       |
| **LoRa RST**           | PB\_4             | P9.15 (GPIO\_48)         |
| **LoRa IRQ**           | -	             | P9.12 (GPIO\_60)         |
| **OLED I2C SCL**       | PB\_8             | -                        |
| **OLED I2C SDA**       | PB\_9             | -                        |
| **Servo PWM**          | TIM3\_CH1 (PC\_6) | -                        |
| **Ultrasonic Echo**    | PB\_1             | -                        |
| **Ultrasonic Trigger** | PB\_2             | -                        |

## 📝 Code Structure

```
📂 project-repo/
│️── 📂 stm32/                # STM32F7 Firmware (LoRa, PWM, Sensors)
│️   ├── main.c               # Main logic (sensor reading, LoRa TX)
│️   ├── loRa.c               # LoRa communication functions
│️   ├── servoMotor.c         # Servo PWM control
│️   ├── oledDisplay.c        # OLED I2C display functions
│️   ├── ultrasonic.c         # Ultrasonic sensor distance measurement
│️   ├── FreeRTOS.c           # Task management
│️
│️── 📂 bbb/                  # BeagleBone Black LoRa Receiver
│️   ├── lora_receiver.py     # Python script to receive LoRa packets
│️
│️── README.md                # Project Documentation
```

## ⚡ How to Run the Project

### **1️⃣ Flashing STM32 Code**

1. Open **STM32CubeIDE**.
2. Connect STM32 via USB.
3. Compile & Upload firmware.
4. Connect the Power Bank to STM32.

### **2️⃣ Setting up BeagleBone Black**

1. Install dependencies:
   ```sh
   sudo apt update
   sudo apt install python3-pip
   pip3 install spidev
   ```
2. Enable SPI:
   ```sh
   config-pin P9.17 spi_cs
   config-pin P9.18 spi
   config-pin P9.21 spi
   config-pin P9.22 spi
   ```
3. Run the LoRa Receiver:
   ```sh
   sudo python3 lora_receiver.py
   ```

## 🎯 Future Improvements

- 🔹 Add **LoRa Acknowledgment (ACK)** for reliable transmission.
- 🔹 Optimize **data packet structure** for efficiency.
- 🔹 Running Jetson Nano Web Server.

## 💡 Contributors

- **Asaad Morani** - Embedded Systems Engineer

---

🚀 **This README ensures that anyone visiting My GitHub can quickly understand and run the project!** 🎯


