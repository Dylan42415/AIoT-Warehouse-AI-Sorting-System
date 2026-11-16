# Machine Learning Carrier V1.0

The **Arduino Machine Learning Carrier V1.0** (Schematic ID: `MachineLearningCarrierV1.0.SchDoc`, Revision: 1.0, Date: 22/01/2021 by Arturo Guadalupi & Dario Pennisi) is the dedicated hardware interface expansion board for the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]]. It breaks out board pins into standardized Grove-compatible connectors, camera socket, and pushbutton interface for [[Theory-TinyML-Kit-Hardware-and-Software|TinyML applications]].

---

## 1. Schematic Architecture & Connectors Breakdown

The schematic provides mating headers (`JANALOG1` and `JDIGITAL1`) that receive the Arduino Nano 33 BLE Sense, breaking out signals to the following dedicated connectors:

```
                  +-----------------------------------+
                  |   Machine Learning Carrier V1.0   |
                  |                                   |
    [ J7: VIN/GND ]                                   |
                  |     +-----------------------+     |
    [ J1: A6/A7 ] |     |  Arduino Nano 33 BLE  |     |
    [ J2: D11   ] |     |     Sense Socket      |     |
    [ J3: D12   ] |     +-----------------------+     |
                  |                                   |
    [ J4: I2C   ] |   [ J8: 20-Pin Camera ]           |
    [ J5: I2C   ] |   [ Connector (OV7675) ]          |
    [ J6: I2C   ] |   [ PB1: Pushbutton (D12) ]       |
                  +-----------------------------------+
```

### Complete Connector Pinout Specification:

| Connector | Part / Type | Pin 1 | Pin 2 | Pin 3 | Pin 4 | Intended Application |
| :--- | :--- | :---: | :---: | :---: | :---: | :--- |
| **`J1`** | 4-Pin Header | `A6` (Analog In 6) | `A7` (Analog In 7) | `+3V3` | `GND` | Dual Analog Inputs / Software UART (`UART_Basic`) |
| **`J2`** | 4-Pin Header | `D11` (MOSI) | NC (Unconnected) | `+3V3` | `GND` | Digital Peripheral / High-speed SPI |
| **`J3`** | 4-Pin Header | `D12` (MISO) | NC (Unconnected) | `+3V3` | `GND` | Digital Peripheral / Button signal tap |
| **`J4`** | 4-Pin Header | `SCL` (A5) | `SDA` (A4) | `+3V3` | `GND` | External I2C Bus Expansion 1 |
| **`J5`** | 4-Pin Header | `SCL` (A5) | `SDA` (A4) | `+3V3` | `GND` | External I2C Bus Expansion 2 |
| **`J6`** | 4-Pin Header | `SCL` (A5) | `SDA` (A4) | `+3V3` | `GND` | External I2C Bus Expansion 3 |
| **`J7`** | 2-Pin Screw Terminal (`691157710002`)| `GND` | `VIN` | N/A | N/A | External Power Input (Battery or DC supply) |
| **`J8`** | 20-Pin Camera Header (`929975-01-10-RK`)| See Table Below | See Table Below | - | - | Dedicated Socket for [[OV7675-Camera-Module|OV7675 Camera]] |

---

## 2. J8 Camera Connector Pinout (OV7675 Interface)

Connector `J8` provides the 8-bit parallel Digital Video Port (DVP), clock lines, and SCCB control signals for the [[OV7675-Camera-Module|OV7675]] sensor:

| Pin # | Signal Name | Connected Nano Pin | Functional Description |
| :---: | :--- | :--- | :--- |
| **1** | `+3V3` | `+3V3` | 3.3V Regulated Power Supply |
| **2** | `GND` | `GND` | Power Ground |
| **3** | `SCL` | `A5` / `P0.02` | SCCB / I2C Clock |
| **4** | `SDA` | `A4` / `P0.31` | SCCB / I2C Data |
| **5** | `VSYNC` | `D4` / `P1.11` | Vertical Frame Synchronization |
| **6** | `HREF` / `HSYNC`| `D3` / `P1.10` | Horizontal Reference / Line Synchronization |
| **7** | `PCLK` | `D2` / `P1.03` | Pixel Clock Output |
| **8** | `XCLK` | `D9` / `P0.27` | Master System Clock Input |
| **9** | `D7` (MSB) | `D6` / `P1.15` | Video Data Bit 7 |
| **10**| `D6` | `D5` / `P1.12` | Video Data Bit 6 |
| **11**| `D5` | `D8` / `P1.14` | Video Data Bit 5 |
| **12**| `D4` | `D7` / `P1.13` | Video Data Bit 4 |
| **13**| `D3` | `D10` / `P1.02` | Video Data Bit 3 |
| **14**| `D2` | `D11` / `P1.01` | Video Data Bit 2 (via SJ1) |
| **15**| `RESET` | `RESETN` | Camera Hardware Reset |
| **16**| `PWDN` | `GND` | Power Down Control (Tied to GND for Active) |
| **17**| NC | Unconnected | No Connection |
| **18**| NC | Unconnected | No Connection |
| **19**| NC | Unconnected | No Connection |
| **20**| NC | Unconnected | No Connection |

---

## 3. Pushbutton Circuitry (PB1) & Solder Jumpers (SJ1, SJ2)

- **Pushbutton `PB1`:** Connected directly to digital pin **`D12`** (`P1.08`) and routed to Ground. When pressed, `PB1` pulls `D12` to `GND` (`LOW`). An internal or external pull-up resistor to `+3V3` maintains `HIGH` state when unpressed. Used in [[Lab-13-Button-Interrupt|Lab 13 Button Interrupt]].
- **Solder Jumpers `SJ1` and `SJ2`:** Allow re-routing or isolating camera data lines (`D2`, `D3`) when using pins `D11` or `D12` for general-purpose digital input/output.

---

## Traceability & Source Excerpts

> **Source:** `04_Hardware_Documents/MachineLearningCarrierV1.0.pdf` (Page 1 Schematic: `MachineLearningCarrierV1.0.SchDoc`)
> 
> "Title: MachineLearningCarrierV1.0, Revision: 1.0, Date: 22/01/2021, Author: Arturo Guadalupi, RevAuthor: Dario Pennisi.
> Connectors: J1 (A6, A7, +3V3, GND); J2 (D11, NC, +3V3, GND); J3 (D12, NC, +3V3, GND); J4/J5/J6 (SCL, SDA, +3V3, GND); J7 (VIN, GND screw terminal 691157710002); J8 (Camera connector 929975-01-10-RK); PB1 pushbutton connected to D12 and GND."
