# STM32F103C8T6 RFID Access Control System

This project implements an RFID-based access control system using the STM32F103C8T6 microcontroller. It integrates an RC522 RFID module for card scanning, a 16x2 I2C LCD for status display, a 4x4 keypad for password management, and a servo motor to control a door lock. The system allows authorized RFID cards (`masterCard` and `slaveCards`) to open the door and supports password changes via the keypad when triggered by the master card.

## Features
- **RFID Card Scanning**: Reads MIFARE Classic 1K/4K cards using the RC522 module via SPI.
- **Access Control**: Opens the door (servo to 90°) for 2 seconds when a valid `masterCard` or `slaveCard` is scanned.
- **Password Management**: Changes a 4-digit password using the keypad, triggered by the master card (key '8').
- **LCD Display**: Shows status messages (e.g., "Access Granted", "Scan a Card") on a 16x2 I2C LCD.
- **Non-Blocking Operation**: Polls RFID every ~100 ms to ensure keypad responsiveness.

## Hardware Requirements
- **Microcontroller**: STM32F103C8T6 (Blue Pill or similar).
- **RFID Module**: RC522 (SPI interface).
- **LCD**: 16x2 I2C LCD (PCF8574 backpack, address 0x27 or 0x3F).
- **Keypad**: 4x4 matrix keypad.
- **Servo Motor**: SG90 or similar (PWM-controlled).
- **Power Supply**: 3.3V for STM32 and RC522, 5V for servo and LCD (optional external supply).
- **Miscellaneous**: Breadboard, jumper wires, 10kΩ resistor (optional for MISO pull-up).

## Software Requirements
- **IDE**: STM32CubeIDE (or similar).
- **HAL Library**: STM32CubeF1 HAL for STM32F103C8T6.
- **Compiler**: ARM GCC (included with STM32CubeIDE).
- **Programmer**: ST-Link V2 or USB-to-serial adapter for flashing.

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
| Keypad Cols | PB7, PB8, PB9, PB10      | Input pins with pull-up            |
| Servo PWM   | PA8                      | TIM1 Channel 1 (PWM)               |
| Servo VCC   | 5V                       | External 5V if STM32 supply is weak |
| Servo GND   | GND                      | Common ground                      |

## Usage
- **Startup**:
  - LCD shows “Booting…” (1 s), “Ver:91” or “Ver:92” (2 s, RC522 version), then “Scan a Card” (row 1), “Ready” (row 2).
  - Servo initializes to 0° (door closed).
- **Normal Operation**:
  - Scan an RFID card:
    - **Valid Card** (`masterCard` or `slaveCards`): LCD shows “Access Granted” and UID, servo moves to 90° (2 s), then back to 0°.
    - **Invalid Card**: LCD shows “Access Denied” and UID (2 s).
  - LCD reverts to “Scan a Card”/“Ready”.
- **Password Change**:
  - Press ‘8’ on keypad: LCD shows “Scan Master Card”.
  - Scan `masterCard`: LCD prompts “Enter Password:”.
  - Enter current 4-digit password (default: “1234”).
  - If correct, LCD prompts “New Password:”. Enter new 4-digit password.
  - Success: LCD shows “Password Changed” (2 s).
  - Failure: LCD shows “Wrong Password” or “Invalid Card” (2 s).
- **Error Handling**:
  - If RC522 fails to initialize, LCD shows “RFID Failure” and halts.

## File Structure
```
your-repo-name/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── I2C_LCD.h
│   │   ├── I2C_LCD_cfg.h
│   │   ├── rc522.h
│   │   ├── KEYPAD.h
│   │   ├── KEYPAD_cfg.h
│   │   ├── SERVO.h
│   ├── Src/
│   │   ├── main.c
│   │   ├── I2C_LCD.c
│   │   ├── I2C_LCD_cfg.c
│   │   ├── rc522.c
│   │   ├── KEYPAD.c
│   │   ├── KEYPAD_cfg.c
│   │   ├── SERVO.c
│   │   ├── SERVO_cfg.c
├── .ioc                # STM32CubeMX configuration (optional)
├── README.md
├── LICENSE
```

- **main.c**: Main program with RFID scanning, servo control, keypad input, and LCD output.
- **I2C_LCD.h/c/cfg**: I2C LCD driver and configuration for 16x2 display.
- **rc522.h/c**: RC522 RFID driver for SPI communication.
- **KEYPAD.h/c/cfg**: 4x4 keypad driver and configuration.
- **SERVO.h/c/cfg**: Servo motor driver and configuration for PWM control.

## Troubleshooting
- **LCD Shows “RFID Failure”**:
  - Check RC522 wiring (PB12: CS, PB0: RST, PB13–PB15: SPI).
  - Verify 3.3V supply (~100–150 mA).
  - Add MISO pull-up resistor (10kΩ on PB14).
  - Check RC522 version in `main.c`:
    ```c
    snprintf(displayText, sizeof(displayText), "Ver:%02X", chipVersion);
    I2C_LCD_WriteString(&LCD_DEVICE, displayText);
    HAL_Delay(5000);
    ```
    Expected: “Ver:91” or “Ver:92”.
- **No Card Detection**:
  - Ensure MIFARE Classic 1K/4K cards.
  - Lower SPI baud rate in `MX_SPI2_Init`:
    ```c
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    ```
  - Verify `rc522.c` timeout (`i = 500` in `MFRC522_ToCard`).
- **Keypad Unresponsive**:
  - Check wiring (PB3–PB6: rows, PB7–PB10: columns).
  - Test keypad:
    ```c
    char key = KEYPAD_GetKey(&KEYPAD);
    if (key != '\0') {
        snprintf(displayText, sizeof(displayText), "Key:%c", key);
        I2C_LCD_WriteString(&LCD_DEVICE, displayText);
        HAL_Delay(1000);
    }
    ```
- **Servo Not Moving**:
  - Check PA8 (PWM), 5V, GND.
  - Use external 5V for servo.
  - Test servo:
    ```c
    SERVO_SetAngle(&SERVO, 90);
    HAL_Delay(2000);
    SERVO_SetAngle(&SERVO, 0);
    ```
- **LCD Blank**:
  - Verify I2C address (0x27 or 0x3F) in `I2C_LCD_cfg.c`.
  - Check PB10 (SCL), PB11 (SDA), VCC.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing
Feel free to submit issues or pull requests for improvements, bug fixes, or additional features.

## Acknowledgments
- STM32CubeF1 HAL library for STM32F103C8T6.
- RC522 library adapted for STM32 HAL.
- DeepBlueMbedded (www.DeepBlueMbedded.com) for keypad, LCD, and servo drivers.