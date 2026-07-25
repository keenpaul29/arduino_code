/*
 * =====================================================================================
 *  8x8 LED MATRIX WITH MAX7219 DRIVER — COMPLETE DOCUMENTATION & IMPLEMENTATION
 * =====================================================================================
 * 
 * 1. THE CORE HARDWARE & MULTIPLEXING PROBLEM:
 *    - An 8x8 LED matrix consists of 64 individual Light Emitting Diodes (8 rows x 8 cols).
 *    - Direct control requires 16 pins (8 row cathode/anode lines + 8 column lines).
 *    - Direct driving hogs microcontroller I/O pins and requires rapid manual multiplexing 
 *      in code to switch rows/cols faster than human vision persistence (~60Hz).
 * 
 * 2. THE MAX7219 DRIVER SOLUTION:
 *    - A specialized serial-input/parallel-output display driver chip.
 *    - Handles all 64 LEDs and internal refresh multiplexing in onboard static RAM.
 *    - Reduces the required Arduino microcontroller communication lines to just 3 pins.
 * 
 * 3. SPI-LIKE 3-WIRE COMMUNICATION BUS:
 *    - DIN (Data In)  : Serial data line sending byte streams (MOSI).
 *    - CLK (Clock)    : Synchronizes bit timing between Arduino and MAX7219 (SCK).
 *    - CS  (Load/SS)  : Latch signal; toggling HIGH locks in received data packets.
 * 
 * 4. PIN CONNECTIONS FOR THIS SKETCH:
 *    +-------------------+--------------------+---------------------------------------+
 *    | MAX7219 Module Pin| Arduino Uno Pin    | Function Description                  |
 *    +-------------------+--------------------+---------------------------------------+
 *    | VCC               | 5V                 | 5V Operating Power (Power LEDs)       |
 *    | GND               | GND                | Common System Ground Reference        |
 *    | DIN (Data In)     | Digital Pin 4      | Transmits bitmap rows (Serial Data)   |
 *    | CS / LOAD         | Digital Pin 11     | Chip Select / Data Latch Trigger      |
 *    | CLK (Clock)       | Digital Pin 7      | Serial Data Synchronizing Clock       |
 *    +-------------------+--------------------+---------------------------------------+
 * 
 * 5. POWER CONSIDERATIONS & DAISY CHAINING:
 *    - Power Draw : Single matrix consumes ~100mA - 160mA at maximum brightness (safe for 5V pin).
 *    - Expansion  : Multiple matrices can be daisy-chained by linking DOUT -> DIN, CLK -> CLK, 
 *                   CS -> CS, VCC -> VCC, and GND -> GND. Use an external 5V supply if 
 *                   chaining 3+ modules to prevent brownouts.
 * 
 * 6. BITMASK GRAPHIC REPRESENTATION:
 *    - Each 8-bit byte represents 1 horizontal row (8 columns = 8 bits).
 *    - Binary '1' = LED ON  (Current flows through diode).
 *    - Binary '0' = LED OFF (No current).
 * =====================================================================================
 */

#include "LedControl.h" // Requires the "LedControl" library by Eberhard Fahle

// Custom Digital Pin Assignments
const int DIN_PIN = 4;  // Serial Data Input
const int CS_PIN  = 11; // Chip Select / Latch
const int CLK_PIN = 7;  // Serial Clock

/*
 * Initialize LedControl Object:
 * LedControl(dataPin, clockPin, csPin, numDevices)
 * Here: DIN=4, CLK=7, CS=11, Controlling 1 Matrix (Device Index 0)
 */
LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

// -------------------------------------------------------------------------------------
// ANIMATION BITMAP PATTERNS (Concentric Shrinking Boxes)
// -------------------------------------------------------------------------------------

// Frame 1: Outer 8x8 Boundary Box
const byte BOX_LARGE[8] = {
  B11111111, // Row 0: All 8 LEDs ON
  B10000001, // Row 1: Outer edges ON, middle 6 OFF
  B10000001, // Row 2
  B10000001, // Row 3
  B10000001, // Row 4
  B10000001, // Row 5
  B10000001, // Row 6
  B11111111  // Row 7: All 8 LEDs ON
};

// Frame 2: Middle 6x6 Box
const byte BOX_MEDIUM[8] = {
  B00000000, // Row 0: Fully OFF
  B01111110, // Row 1: 6-LED line centered
  B01000010, // Row 2
  B01000010, // Row 3
  B01000010, // Row 4
  B01000010, // Row 5
  B01111110, // Row 6
  B00000000  // Row 7: Fully OFF
};

// Frame 3: Inner 4x4 Box
const byte BOX_SMALL[8] = {
  B00000000,
  B00000000,
  B00111100, // Row 2: 4-LED line centered
  B00100100, // Row 3
  B00100100, // Row 4
  B00111100, // Row 5
  B00000000,
  B00000000
};

// Frame 4: Center 2x2 Core
const byte BOX_DOT[8] = {
  B00000000,
  B00000000,
  B00000000,
  B00011000, // Row 3: Center 2 LEDs ON
  B00011000, // Row 4: Center 2 LEDs ON
  B00000000,
  B00000000,
  B00000000
};

/*
 * HELPER FUNCTION: renderFrame
 * Takes a 1D array of 8 byte bitmasks and pushes them row-by-row
 * to Matrix Index 0 via SPI bit shift transfers.
 */
void renderFrame(const byte pattern[]) {
  for (int row = 0; row < 8; row++) {
    // display.setRow(deviceIndex, rowNumber, byteValue)
    display.setRow(0, row, pattern[row]);
  }
}

void setup() {
  /*
   * MAX7219 INITIALIZATION SEQUENCE:
   * 1. Wake up driver from hardware low-power shutdown mode (default on startup).
   * 2. Set LED brightness intensity (0 = minimum/dim, 15 = maximum brightness).
   * 3. Clear existing display memory buffer.
   */
  display.shutdown(0, false); // Device 0: false = Normal Operation Mode
  display.setIntensity(0, 5); // Device 0: Brightness Level 5 out of 15
  display.clearDisplay(0);    // Device 0: Flush row RAM registers
}

void loop() {
  /*
   * ANIMATION LOOP:
   * Sequentially renders 4 collapsing frame bitmaps with 150ms delays,
   * creating a smooth repeating shrinking box visual effect.
   */
  renderFrame(BOX_LARGE);
  delay(150);
  
  renderFrame(BOX_MEDIUM);
  delay(150);
  
  renderFrame(BOX_SMALL);
  delay(150);
  
  renderFrame(BOX_DOT);
  delay(150);
}