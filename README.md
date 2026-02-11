## FT-TT-CAN-FD
firmware for calculated ft tt tables on fdcan for stm32 g4 series

Overview
This research firmware implements a **Fault-Tolerant Time-Triggered CAN FD (FT-TTCAN-FD)** communication scheme on an **STM32 microcontroller**.  
The system uses a **user button** to start the time-triggered schedule and relies on **timer output-compare channels** to generate synchronization events for the TTCAN cycle.

The goal is to create a deterministic, synchronized communication framework with support for time-triggered scheduling and fault-tolerant synchronization.


## Research Objective
- Implement a **time-triggered CAN FD schedule** on STM32.
- Use **hardware timers** for precise synchronization.
- Study **latency, jitter, and synchronization accuracy**.
- Provide a base for **fault-tolerant TTCAN-FD experiments**.

![WhatsApp Image 2026-02-10 at 15 39 56](https://github.com/user-attachments/assets/742165b0-be32-449a-babf-8b48a75585d6)

## Key Features
- User button to start TTCAN schedule.
- Hardware timer-based global time reference.
- Output-compare channels used for:
  - Sync pulse generation.
  - Slot timing events.
- Time-triggered message transmission.
- Deterministic scheduling.
- Support for CAN FD frames.
- Foundation for fault-tolerant extensions.
