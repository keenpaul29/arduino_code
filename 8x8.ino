/*
 * =======================================================================================
 * PROJECT: 8x8 LED Matrix Control using MAX7219 IC & LedControl Library
 * =======================================================================================
 * 
 * --- THEORY & HARDWARE OVERVIEW ---
 * 1. MAX7219 LED Driver:
 *    An 8x8 LED matrix contains 64 individual LEDs. Controlling them directly would require 
 *    16 microcontroller pins (8 rows + 8 columns). The MAX7219 IC acts as a multiplexed 
 *    display driver that allows driving all 64 LEDs using only 3 digital control pins.
 * 
 * 2. Communication Protocol (SPI-like Serial Interface):
 *    - DIN (Data In)   : Serial Data line. Transfers bits one-by-one into the MAX7219 shift register.
 *    - CLK (Clock)     : Synchronizes data transmission. On every clock pulse, one bit is shifted in.
 *    - CS / LOAD (Chip Select) : Latches data. When CS goes HIGH, data inside the shift register is loaded into the internal display RAM.
 * 
 * 3. Matrix Mapping & Binary Representation:
 *    - Each row in an 8x8 matrix is controlled by an 8-bit byte.
 *    - Binary prefix '0b' represents 8 LEDs across a single row:
 *      '1' = LED ON (HIGH)  |  '0' = LED OFF (LOW)
 * =======================================================================================
 */

#include <LedControl.h> // Include library for handling MAX7219 / MAX7221 display drivers

// --- PIN ASSIGNMENTS ---
int d_in = 4; // Serial Data Input pin (DIN -> Microcontroller Pin 4)
int clk  = 8; // Serial Clock pin (CLK -> Microcontroller Pin 8)
int cs   = 7; // Chip Select / Latch pin (CS/LOAD -> Microcontroller Pin 7)

/*
 * LedControl Object Initialization
 * Syntax: LedControl(DataIn, CLK, CS, numDevices)
 * - numDevices = 1: Defines how many MAX7219 ICs are daisy-chained together (1 to 8).
 */
LedControl lc = LedControl(d_in, clk, cs, 1);

/*
 * BITMAP ARRAY (8 bytes = 64 pixels total)
 * Index [0] = Top Row (Row 0) down to Index [7] = Bottom Row (Row 7).
 *
 * Visual layout of the smiley pattern below:
 *  Row 0: [1 1 1 1 1 1 1 1] -> Top Border
 *  Row 1: [1 0 0 0 0 0 0 1] -> Side Borders
 *  Row 2: [1 0 1 0 0 1 0 1] -> Eyes
 *  Row 3: [1 0 0 0 0 0 0 1] -> Nose/Cheek Space
 *  Row 4: [1 1 0 0 0 0 1 1] -> Mouth Edges
 *  Row 5: [1 0 1 1 1 1 0 1] -> Smile Curve
 *  Row 6: [1 0 0 0 0 0 0 1] -> Side Borders
 *  Row 7: [1 1 1 1 1 1 1 1] -> Bottom Border
 */
byte smiley[8] = {
  0b11111111, // Row 0
  0b10000001, // Row 1
  0b10100101, // Row 2
  0b10000001, // Row 3
  0b11000011, // Row 4
  0b10111101, // Row 5
  0b10000001, // Row 6
  0b11111111  // Row 7
};

void setup() {
  /*
   * POWER-SAVING MODE (Shutdown Register):
   * Upon startup, the MAX7219 enters Power Shutdown Mode by default to save energy.
   * Parameter 1: Device address (0 for the 1st matrix).
   * Parameter 2: 'false' disables shutdown mode (wakes up the display).
   */
  lc.shutdown(0, false);

  /*
   * BRIGHTNESS CONTROL (Intensity Register):
   * The MAX7219 uses internal Pulse-Width Modulation (PWM) to adjust display brightness.
   * Parameter 1: Device address (0).
   * Parameter 2: Brightness level from 0 (minimum intensity) to 15 (maximum intensity).
   */
  lc.setIntensity(0, 8);

  /*
   * DISPLAY CLEARING:
   * Flushes all display RAM registers inside the MAX7219 to turn off all 64 LEDs,
   * preventing leftover garbage data from displaying on boot.
   */
  lc.clearDisplay(0);
}

void loop() {
  /*
   * MATRIX UPDATE LOOP:
   * Iterates through rows 0 to 7, writing each byte from the 'smiley' array 
   * into the corresponding row register of the MAX7219 display driver.
   * 
   * Function: lc.setRow(deviceIndex, rowIndex, byteValue)
   * - deviceIndex = 0 (1st matrix)
   * - rowIndex    = i (0 through 7)
   * - byteValue   = smiley[i] (8-bit binary pattern for row 'i')
   */
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, smiley[i]);
  }
}