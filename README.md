# 🏎️ Robo Rumble 2026: Autonomous 4WD Maze Racing Rover

> **Category:** Robo Grand Prix (Autonomous Racing)  
> **Platform:** 4WD Autonomous Mobile Robot  
> **Navigation:** Dynamic Wall-Sensing & Corridor Centering (No Line Tracking)  
> **Compliance:** Footprint $\le 50\text{ cm} \times 50\text{ cm}$ | Mass $< 5\text{ kg}$ | Integrated E-Stop Circuit  

---

## 📌 High-Level Project Summary

### What We Are Building
This project is an **autonomous 4WD mobile maze-racing rover** custom-engineered to compete in the **Robo Grand Prix** category at Robo Rumble 2026. Designed to navigate complex maze tracks without relying on surface line-following, the vehicle dynamically maps wall boundaries in real time, executes precision cornering, avoids dead ends, and maintains high velocity through narrow corridors—operating 100% autonomously with zero human intervention or remote control.

### How We Are Building It
* **Mechanical Frame:** A custom dual-deck lightweight chassis optimized for structural rigidity, staying well within the maximum $50\text{ cm} \times 50\text{ cm}$ footprint and $5\text{ kg}$ weight limit.
* **Drivetrain & Steering:** 4-wheel independent motor drive running on a differential skid-steering system managed by high-efficiency dual H-bridge motor drivers.
* **Perception & Ranging:** A multi-sensor array (Ultrasonic / Time-of-Flight distance sensors) mounted at the front, left, and right to measure corridor clearance and detect upcoming turns in real time.
* **Navigation & Control Architecture:** Embedded C++ firmware utilizing a closed-loop Proportional-Integral-Derivative (PID) wall-following algorithm combined with threshold logic for intersection decision-making.
* **Power & Safety Integrity:** Dual power management system separating microcontroller logic from high-current motor draws, featuring an accessible physical ON/OFF switch and a hardware-integrated **Emergency Stop (E-Stop)** circuit.

---

## 📂 Directory Map

```text
robo-rumble-2026-grand-prix/
│
├── 📁 Folder A - Source Code/           # Firmware, control algorithms, & logic diagrams
│   ├── 📄 src/                         # Arduino / C++ source code files
│   ├── 📄 flowcharts/                  # Navigation & maze decision logic flowcharts
│   └── 📄 README.md                    # Detailed software framework documentation
│
├── 📁 Folder B - Designs/               # Physical CAD, electrical schematics, & simulations
│   ├── 📄 Mechanical Design/           # 3D CAD renders, chassis assembly, & CAD files
│   ├── 📄 Electronic Design/           # Component selections & hardware justifications
│   ├── 📄 Schematics/                  # Annotated wiring schematics with E-Stop & Switch
│   └── 📄 Simulation/                  # Circuit & kinematic simulation test results
│
└── 📁 Folder C - Documentation/         # Submission files, budget, & attendance logs
    ├── 📄 Pitch Deck.pdf                # Official 7-slide project presentation
    ├── 📄 Bill_of_Materials.xlsx        # Itemized spreadsheet (Total Cost in RED)
    ├── 📄 Holistic_Build_Document.md    # Comprehensive technical engineering report
    ├── 📄 FQA_Attendance_Log.md         # Facilitator Q&A attendance logbook
    └── 📁 FQA_Proof/                    # Verified attendance proof screenshots
