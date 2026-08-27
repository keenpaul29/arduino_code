/*
  ==============================================================================
  THEORY & WORKING PRINCIPLE: 16x2 PARALLEL (NON-I2C) LCD INTERFACING
  ==============================================================================
  1. HD44780 CONTROLLER ARCHITECTURE:
     - Standard character LCDs use the Hitachi HD44780 driver chip.
     - The display contains 2 rows of 16 character blocks. Each block is a 5x8 pixel grid.
     - It relies on two internal registers:
       * Instruction Register (IR): Receives commands (clear screen, set cursor position, etc.).
       * Data Register (DR): Receives character ASCII codes to render onto the display.

  2. 4-BIT vs. 8-BIT PARALLEL BUS MODES:
     - 8-Bit Mode: Uses 8 data lines (D0-D7) to transfer a full byte of data in 1 clock cycle.
       Requires 11 total I/O pins on the microcontroller.
     - 4-Bit Mode: Uses only 4 data lines (D4-D7). A 8-bit byte is split into two 4-bit 
       halves ("nibbles") sent sequentially (High Nibble first, then Low Nibble).
       Saves 4 microcontroller pins (uses 7 total I/O pins) with negligible speed impact.

  3. LCD CONTROL SIGNALS:
     - RS (Register Select): Toggles between Command Mode (RS = LOW) and Data Mode (RS = HIGH).
     - RW (Read/Write): Selects Read mode (HIGH) or Write mode (LOW). Hardwiring RW directly 
       to GND locks the LCD into permanent Write Mode, saving an extra Arduino pin.
     - E (Enable): Acts as the clock signal. A HIGH-to-LOW pulse (falling edge) triggers 
       the HD44780 chip to latch and process the data present on lines D4–D7.

  4. PINOUT & HARDWARE WIRING SUMMARY:
     ------------------------------------------------------------------
     LCD Pin  | Symbol | Function                | Connection
     ------------------------------------------------------------------
     Pin 1    | VSS    | Ground                  | Arduino GND
     Pin 2    | VDD    | Power Supply (5V)       | Arduino 5V
     Pin 3    | V0     | Contrast Adjust         | Potentiometer Center Wiper
     Pin 4    | RS     | Register Select         | Arduino Digital Pin 12
     Pin 5    | RW     | Read/Write              | Arduino GND (Write Mode)
     Pin 6    | E      | Enable Signal           | Arduino Digital Pin 11
     Pin 7-10 | D0-D3  | Lower Data Bus Pins     | Left Unconnected (4-bit mode)
     Pin 11   | D4     | Data Bit 4              | Arduino Digital Pin 5
     Pin 12   | D5     | Data Bit 5              | Arduino Digital Pin 4
     Pin 13   | D6     | Data Bit 6              | Arduino Digital Pin 3
     Pin 14   | D7     | Data Bit 7              | Arduino Digital Pin 2
     Pin 15   | A/LED+ | Backlight Anode (+5V)   | 5V (via 220Ω Resistor)
     Pin 16   | K/LED- | Backlight Cathode (GND) | Arduino GND
     ------------------------------------------------------------------
  ==============================================================================
*/

// Include the built-in Arduino LiquidCrystal library designed for parallel HD44780 displays
#include <LiquidCrystal.h>

// Assign hardware pin connections: RS, E, D4, D5, D6, D7
const int RS_PIN = 12;
const int E_PIN  = 11;
const int D4_PIN = 5;
const int D5_PIN = 4;
const int D6_PIN = 3;
const int D7_PIN = 2;

// Instantiate the LCD object passing the pin configuration.
// Order required by library: LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(RS_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

void setup() {
    // Initialize the LCD interface and specify display dimensions (16 columns, 2 rows).
    // This command sends initial 4-bit configuration sequence bytes to the HD44780.
    lcd.begin(16, 2);

    // Clear any residual data stored in display RAM and set cursor to home position (0,0)
    lcd.clear();
}

void loop() {
    // Set cursor to Column 0, Row 0 (Top line)
    // Note: Coordinates are 0-indexed: setCursor(col, row)
    lcd.setCursor(0, 0);
    
    // Write string to LCD Data Register (DR); automatically advances cursor per character
    lcd.print("Hello, World!");

    // Set cursor to Column 0, Row 1 (Bottom line)
    lcd.setCursor(0, 1);
    lcd.print("Parallel LCD");
}
