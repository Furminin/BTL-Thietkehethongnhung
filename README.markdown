# STM32F103C8T6 RFID Access Control System

This project implements an RFID-based access control system using the STM32F103C8T6 microcontroller. It integrates an RC522 RFID module for card scanning, a 16x2 I2C LCD for status display, a 4x4 keypad for password management, and a servo motor to control a door lock. The system allows authorized RFID cards (`masterCard` and `slaveCards`) to open the door and supports password changes via the keypad when triggered by the master card.

## Features
- **RFID Card Scanning**: Reads MIFARE Classic 1K/4K cards using the RC522 module via SPI.
- **Access Control**: Opens the door (servo to 90°) for 2 seconds when a valid `masterCard` or `slaveCard` is scanned.
- **Password Management**: Changes a 6-digit password using the keypad, triggered by the master card (key '8').
- **LCD Display**: Shows status messages (e.g., "Access Granted", "Scan a Card") on a 16x2 I2C LCD.
- **Non-Blocking Operation**: Polls RFID every ~100 ms to ensure keypad responsiveness.

## Hardware Requirements
- **Microcontroller**: STM32F103C8T6 Blue Pill
- **RFID Module**: RC522 (SPI interface).
- **LCD**: 16x2 I2C LCD (PCF8574 backpack, address 0x27 or 0x3F).
- **Keypad**: 4x4 matrix keypad.
- **Servo Motor**: SG90 or similar (PWM-controlled).
- **Power Supply**: 3.3V for STM32 and RC522, 5V for servo and LCD (optional external supply).

## Software Requirements
- **IDE**: STM32CubeIDE (or similar).
- **HAL Library**: STM32CubeF1 HAL for STM32F103C8T6.
- **Compiler**: ARM GCC (included with STM32CubeIDE).
- **Programmer**: ST-Link V2 for flashing.

## Pin Connections
| Component   | STM32 Pins               | Notes                              |
|-------------|--------------------------|------------------------------------|
| RC522 CS    | PB12                     | SPI Chip Select                    |
| RC522 RST   | PB0                      | Reset pin                          |
| RC522 SCK   | PB13                     | SPI Clock                          |
| RC522 MISO  | PB14                     | SPI Master In Slave Out            |
| RC522 MOSI  | PB15                     | SPI Master Out Slave In            |
| RC522 VCC   | 3.3V                     | ~100–150 mA                        |
| RC522 GND   | GND                      | Common ground                      |
| LCD SCL     | PB10                     | I2C2 Clock                         |
| LCD SDA     | PB11                     | I2C2 Data                          |
| LCD VCC     | 5V or 3.3V               | Check LCD specs                    |
| LCD GND     | GND                      | Common ground                      |
| Keypad Rows | PB3, PB4, PB5, PB6       | Output pins                        |
| Keypad Cols | PB7, PB8, PB9, PB10      | Input pins with pull-down          |
| Servo PWM   | PA8                      | TIM1 Channel 1 (PWM)               |
| Servo VCC   | 5V                       | External 5V if STM32 supply is weak |
| Servo GND   | GND                      | Common ground                      |

<<<<<<< HEAD
## Block Diagram
The following text-based flowchart illustrates the operational flow of the RFID access control system:

```
+----------------+
|  Start         |
|  (Power On)    |
+----------------+
        |
        v
+---------------------------+
|  Initialize System        |
|  - Configure SPI2 (RC522) |
|  - Configure I2C1 (LCD)   |
|  - Configure TIM1 (Servo) |
|  - Configure TIM2 (Tone)  |
|  - Set Servo to 0°        |
|  - Display "Nhap mat khau:"|
|    or "Mat khau moi:"     |
+---------------------------+
        |
        v
+---------------------------+
|  Poll RFID Module         |
|  - Check for Card (100ms) |
|  - If Card Detected:      |
|    - Read UID             |
+---------------------------+
        |                    |
        | No                 | Yes
        v                    v
+----------------+    +---------------------------+
|  Check Keypad   |    |  Process RFID Card       |
|  Input          |    |  - If addCardMode:       |
|                 |    |    - Prompt "Quet Master Card"|
+----------------+    |    - Validate masterCard  |
        |            |    - If Valid: Prompt "Quet The Moi"|
        |            |    - Add new slaveCard    |
        |            |  - Else:                  |
        |            |    - Validate slaveCard   |
        |            |    - If Valid: "Welcome", |
        |            |      Servo 90° (1.5s),   |
        |            |      Tone Sequence        |
        |            |    - Else: "Sai The",    |
        |            |      Tone (G4)           |
        |            +---------------------------+
        |                    |                    |
        |                    | Add/Access         | Error
        |                    v                    v
        |            +----------------+    +----------------+
        |            |  Return to     |    |  Display      |
        |            |  "Nhap mat khau:"|    |  Error Msg   |
        |            +----------------+    |  (e.g., "Sai Master Card")|
        |                    |            +----------------+
        |                    v
        |            +---------------------------+
        |            |  Check Keypad for 'F3'    |
        |            |  (Add Card Mode)          |
        |            +---------------------------+
        |                    |                    |
        |                    | No                 | Yes
        |                    v                    v
        |            +----------------+    +---------------------------+
| Check Keypad  |    |  Enter Add Card Mode     |
| for Inputs    |    |  - Set addCardMode = 1   |
|               |    |  - masterAuthenticated = 0|
|               |    |  - Display "Quet Master Card"|
+----------------+    +---------------------------+
        |                    |
        | Digits (0-9)       | 'F4'
        v                    v
+----------------+    +----------------+
| Collect 6      |    | Clear Password |
| Digits         |    | Buffer         |
| - Display Each |    | - Reset Cursor |
| - If 6 Digits: |    | - "Nhap mat khau:"|
|   Validate     |    +----------------+
+----------------+           |
        |                    | 'F1'+'F2'
        |                    v
        |            +----------------+
        |            | Enter Password |
        |            | Change Mode    |
        |            | - "Mat khau cu:"|
        |            | - Validate Old |
        |            | - "Mat khau moi:"|
        |            | - Set New      |
        |            | - "Doi thanh cong"|
        |            +----------------+
        |
        | Valid             | Invalid
        v                    v
+----------------+    +---------------------------+
| Password OK     |    | Password Wrong           |
| - "Welcome"     |    | - attemptCount++         |
| - Servo 90°     |    | - "Sai Mat Khau" (2s)    |
|   (1.5s)        |    | - Octave Tone (TIM2)     |
| - Tone Sequence |    | - If attemptCount >= 5:  |
| - Clear Buffer  |    |   - "Khoa 30s" (30s)     |
| - Reset Cursor  |    |   - Reset attemptCount   |
| - "Nhap mat khau:"|    | - Clear Buffer           |
+----------------+    | - Reset Cursor           |
        |            | - "Nhap mat khau:"       |
        |            +---------------------------+
        v                    v
+----------------+    +----------------+
|  End           |    |  End           |
|  (Idle Loop)   |    |  (Idle Loop)   |
+----------------+    +----------------+
        |                    |
        v                    v
+---------------------------+
|  Error Handling           |
|  - If RC522 Fails:        |
|    Display "RFID Failure" |
|    and Halt               |
+---------------------------+
```


======
## Acknowledgments
- STM32CubeF1 HAL library for STM32F103C8T6.
- RC522 library adapted for STM32 HAL.
- DeepBlueMbedded (www.DeepBlueMbedded.com) for keypad, LCD, and servo drivers.
