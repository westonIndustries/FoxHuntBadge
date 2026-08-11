# Mini SAO Pseudo-Doppler Fox-Hunting & Blinky Badge

An ultra-low-cost, open-source, dual-purpose **Pseudo-Doppler Radio Direction Finding (RDF) Fox-Hunting Badge** and **SAO (Shitty Add-On) Conference Badge**. Designed around the cheap and powerful **CH32V003 (RISC-V)** microcontroller, this board seamlessly transitions between an eye-catching conference "blinky badge" and an active $2\text{m} / 70\text{cm}$ RDF bearing and proximity indicator when connected to a handheld radio (e.g., Baofeng UV-5R).

---

## Key Features

* **Dual-Purpose Functionality:**
  * **Default / Standby Mode:** Eye-catching ambient "Blinky Badge" with randomly selected LED patterns (Cylon scanner, spinning radar, rainbow wheel, breathing pulse) on boot-up.
  * **Auto-Detect Active Hunt Mode:** Automatically detects incoming $500\text{ Hz}$ Doppler audio when connected to a radio and transitions instantly into RDF Bearing & Proximity display mode.
* **16-LED RGB Compass Ring:**
  * Displays precise target bearing vector in **Green**.
  * Dynamic "warmer/colder" proximity arc (**Yellow $\rightarrow$ Red**) based on measured signal tone amplitude.
  * Direct GPIO-driven $4 \times 8$ common-anode matrix (no external LED driver IC required).
* **SAO v1.69bis / v4.2 Compatible:**
  * Standard $2 \times 3$ shrouded male connector on the rear for $3.3\text{V}$ power and optional I2C telemetry transmission to host badges.
  * Dual Schottky diode power-ORing prevents USB and host badge backfeeding.
* **Onboard Audio & Listening Options:**
  * Surface-mount passive magnetic buzzer with software PWM volume/tone control and mute functionality.
  * Gold-plated solder pads (L, R, GND) for direct wire connection to earpieces or headphones.
  * **10k$\Omega$ thumbwheel potentiometer** for independent headphone volume adjustment without affecting RDF processing accuracy.
* **Compact & Cost-Optimized:**
  * Form factor: $\sim 45\text{ mm} \times 45\text{ mm}$ double-sided FR4 PCB.
  * Estimated Bill of Materials (BOM) cost: **< $2.50 USD**.

---

## Hardware Architecture

```
                    +----------------------------------------+
                    |             CH32V003 MCU               |
                    |            (TSSOP-20 / RISC-V)         |
                    +---+-+------+------+-------+-------+----+
                        | |      |      |       |       |
      +-----------------+ |      |      |       |       +------------------+
      |                   |      |      |       |                          |
      v                   v      v      |       v                          v
+-----------+    +---------------+    |  +------------+            +---------------+
| 4x RF     |    | 16-LED RGB    |    |  | Tactile    |            | Onboard PWM   |
| Switching |    | Compass Ring  |    |  | Mode Switch|            | Passive       |
| Diodes    |    | (4x8 Matrix)  |    |  | Button     |            | Buzzer        |
+-----+-----+    +---------------+    |  +------------+            +---------------+
      |                               |
      v                               v
+-----------+                +-----------------+                  +----------------+
| 4x SMA    |                | LM358 Active    | <--- 3.5mm Aux   | 10k Thumbwheel |
| Edge      |                | Bandpass Filter |      Audio In    | Volume Pot     |
| Connectors|                +-----------------+      (UV-5R HT)  +-------+--------+
+-----------+                                                             |
                                                                          v
                                                                  +----------------+
                                                                  | Headphone      |
                                                                  | Solder Pads    |
                                                                  +----------------+
```

### Component Breakdown

| Subsystem | Components Selected | Purpose / Function | Est. Cost (USD) |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | CH32V003F4U6 (TSSOP-20) | Central brain: $500\text{ Hz}$ RF switching, audio phase capture, matrix scanning | **$0.15** |
| **USB & Flashing** | CH340N + USB-C Port | Direct USB power and single-wire UART flashing | **$0.28** |
| **RF Switching** | 4× BAP64 / BAV99 Diodes | Fast $2\text{m}/70\text{cm}$ commutating antenna switch | **$0.04** |
| **Audio Conditioning**| LM358 Dual Op-Amp | Active bandpass filter & square-wave limiter | **$0.06** |
| **Display Ring** | 16× 0603 Common-Anode RGB LEDs | Multi-color bearing & proximity display ($4 \times 8$ matrix) | **$0.32** |
| **Audio Output** | 8.5mm SMD Passive Buzzer + Transistor | Audible tone & target alert | **$0.15** |
| **Volume Control** | 10k\Omega$ SMD Thumbwheel Potentiometer | Independent headphone volume adjustment | **$0.10** |
| **User Input** | 3×4mm SMD Tactile Switch | Mode selection & buzzer mute toggle | **$0.02** |
| **Connectors** | 5× SMA Edge Jacks + 3.5mm Aux Jack | Antenna arrays, receiver RF output, radio audio input | **$0.95** |
| **Power Stage** | AP2112K-3.3 LDO + BAT54C Diodes | Dual USB/SAO badge power isolation | **$0.10** |
| **Passives & Board**| 0603 caps/resistors, $45 \times 45\text{ mm}$ PCB | 2-layer FR4 fabrication and passive components | **$0.29** |
| **TOTAL ESTIMATED BOM** | | | **~$2.46 USD** |

---

## Operating Modes

### Mode 0: Blinky Badge (Attract Mode)
* Active when no radio audio cable is plugged in or signal is absent.
* On boot, the microcontroller reads floating ADC noise to randomly select one of four ambient animation patterns:
  1. **Cylon / Knight Rider:** Red/cyan scanner sweeping around the circular arc.
  2. **Radar Sweep:** Rotating green arm with a fading tail.
  3. **Rainbow Wheel:** Smooth 360° color gradient cycling.
  4. **Breathing Pulse:** Soft amber/blue full-ring pulsing.

### Mode 1: Active Fox Hunt (Auto-Triggered on Cable Insertion)
* Automatically activates when a $500\text{ Hz}$ audio signal is detected on the 3.5mm Aux jack.
* **Target Bearing:** Displayed as a solid **Green** LED pointing toward the signal source.
* **Proximity Gauge:** An expanding arc of **Yellow** and **Red** LEDs scales dynamically with measured tone amplitude ("warmer/colder").

### Mode 2: Radar Heatmap
* Full-ring radial color gradient representing distance and signal saturation.

### Mode 3: Minimalist RDF Mode
* Low-brightness, single-LED bearing indication for low power consumption and night hunting.

---

## Interfaces & Connectors

* **5x Edge-Launch SMA Jacks:**
  * 4x Input connections for the 4-element antenna array.
  * 1x Output connection to the handheld radio antenna input.
* **3.5mm Aux Audio Input:** Accepts demodulated speaker audio from the UV-5R or similar HT radio.
* **Headphone Solder Pads (L, R, GND):** Direct wire hookup for earpieces; stereo pads bridged in mono with DC blocking capacitor.
* **Thumbwheel Volume Control:** Adjusts earpiece listening level without degrading signal levels fed to the LM358 processing stage.
* **2x3 Rear SAO Header:**
  * Pin 1: $3.3\text{V}$ Power Input
  * Pin 2: GND
  * Pins 3 & 4: I2C (SDA / SCL) for badge-to-host telemetry
  * Pins 5 & 6: GPIO1 / GPIO2

---

## Firmware Build & Flashing

Built using the open-source **`ch32v003fun`** development environment.

### Prerequisites
* `rv32ec` GCC Toolchain
* `minichlink` or `WCH-Link` programmer (or build-in CH340N USB UART)

### Compilation
```bash
# Clone repository
git clone https://github.com/your-username/sao-fox-hunter.git
cd sao-fox-hunter/firmware

# Compile firmware
make

# Flash via WCH-Link / USB-C
make flash
```

---

## License

Hardware designs and schematics released under **CERN-OHL-P v2**.  
Firmware released under the **MIT License**.