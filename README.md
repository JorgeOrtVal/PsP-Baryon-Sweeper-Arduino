# PsP Baryon Sweeper — Arduino Leonardo

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

> **Unbrick your PSP 3000 using an Arduino Leonardo** — no expensive hardware, no rare diodes, no soldering wizardry required.

---

## Table of Contents

- [What is this?](#what-is-this)
- [Hardware Requirements](#hardware-requirements)
- [Quick Start](#quick-start)
  - [1. Upload the Sketch](#1-upload-the-sketch)
  - [2. Wire the Circuit](#2-wire-the-circuit)
  - [3. Prepare the Memory Stick](#3-prepare-the-memory-stick)
  - [4. Run Baryon Sweeper](#4-run-baryon-sweeper)
- [Repository Contents](#repository-contents)
- [License](#license)

---

## What is this?

This repository contains everything you need to **unbrick a PSP 3000** (or other PSP models) using an **Arduino Leonardo**.

### The Problem

When a PSP gets bricked (corrupted firmware), the Syscon chip enters a special **service mode** waiting for authentication. The standard approach requires a **1N4148 diode** to prevent signal contention between the Arduino and the PSP

### The Solution

The key insight was discovering that **`Serial1.begin(19200)` defaults to 8N1** (8 bits, No parity, 1 stop bit), but the **PSP Syscon speaks 8E2** (8 bits, Even parity, 2 stop bits). By simply using:

```cpp
Serial1.begin(19200, SERIAL_8E2);
```

> 🔬 **Why this matters:** The 1N4148 diode was a workaround for signal contention caused by mismatched UART framing. Fix the framing → fix the communication.

---

## Hardware Requirements

| Component | Notes |
|-----------|-------|
| **Arduino Leonardo** | ✅ Tested and confirmed working |
| **10kΩ resistor** | For the K-line adapter |
| **Jumper wires** (male-female) | 3 wires |
| **PSP 3000** (or other bricked PSP) | Any model with K-line accessible |
| **Memory Stick** | At least 32MB |
| **PSP AC charger** | Required to power the PSP during unbrick |
| **USB cable** | To connect Arduino to PC |

### ❌ NOT Required

- ~~1N4148 diode~~
- ~~Soldering iron~~
- ~~USB-TTL converter~~

---

## Quick Start

### 1. Upload the Sketch

1. Install **Arduino IDE** https://docs.arduino.cc/software/ide/
2. Open **bridge_leonardo/bridge_leonardo.ino**
3. Select **Arduino Leonardo** on the correct **COM port**
5. Press **Upload** (→)

### 2. Wire the Circuit

```
   Arduino Leonardo                    PSP
   ═══════════════════                 ═════
   TX (pin 1) ──┬──[10kΩ]── RX (pin 0) ── K-line (center pin)
                 │
   GND          ──────────────────────── PSP negative (-)
   3.3V         ──────────────────────── PSP positive (+)
```

**⚠️ Important Notes:**

- **K-line** is the **center pin** of the PSP battery connector
- **GND** connects to the **leftmost pin** (negative terminal)
- **3.3V** connects to the **rightmost pin** (positive terminal)
- The **10kΩ resistor** bridges TX and RX, creating the one-wire UART adapter
- **Connect** AREF to RESET (only if you use another Arduino, i can't promise it will work)

### 3. Prepare the Memory Stick

You need a **Magic Memory Stick** with DC-ARK (ARK-4) firmware.

1. Insert your Memory Stick into the PC
2. Run `magic_memory_creator/run_windows.bat` **as Administrator**
3. Select your Memory Stick from the dropdown
4. Click **Run**
5. Wait for "DONE!" — the tool downloads PSP 6.61 firmware, extracts it, and installs IPL + DC-ARK

### 4. Run Baryon Sweeper

1. **Close the Arduino IDE** (especially the Serial Monitor)
2. Open a terminal / command prompt in the `pysweeper/` directory
3. Install dependencies:
   ```bash
   pip install pyserial pycryptodome
   ```
4. Run pysweeper:
   ```bash
   python pysweeper.py
   ```
5. **Configure pysweeper:**
   - Select the **COM port** of your Arduino
   - Select **Service Mode** (0xFFFFFFFF)
   - Click **Start Service**
6. **Connect the PSP:**
   - Insert the **prepared Memory Stick** into the PSP
   - Connect the PSP to its **AC charger**
   - Connect the 3 wires (K-line, GND, and 3.3V) to the PSP battery terminals
7. Wait for pysweeper.py to display multiple lines to confirm it is working (if it shows nothing, even though you started the service, you are doing something wrong or need to leave it connected longer)
8. The PSP should **power on automatically** and boot from the Memory Stick

---

## Repository Contents

```
PsP-Baryon-Sweeper-ArduinoLeonardo/
├── bridge_leonardo/                  
│   └── bridge_leonardo.ino           ← Arduino sketch
│
├── pysweeper/                        
│   └── pysweeper.py                  ← Python tool by khubik
│
└── magic_memory_creator/             
    ├── main.py                       ← GUI for creating Magic Memory Stick
    ├── msipl_installer.py            ← IPL installer module
    ├── requirements.txt              ← Python dependencies
    ├── prep_windows.bat              ← Install dependencies
    ├── run_windows.bat               ← Launch (admin required, no console)
    ├── run_windows_debug.bat         ← Launch with console debug
    └── run_windows_debug_py3.bat     ← Launch with python3 debug
```

## License

⚖️ This project is licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for details.

---

<p align="center">
  <b>If you paid for this software, demand a refund.</b><br>
  <i>This is free, open-source software for the PSP homebrew community.</i>
</p>
