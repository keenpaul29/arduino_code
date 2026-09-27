// ============================================================================
// Program: Optimized 8-Bit Decimal to Binary LED Visualizer
// Description: Uses bit-shifting and pin arrays to display an 8-bit binary
//              value on 8 LEDs and output it over Serial.
// ============================================================================

// Decimal number to convert into binary (Range: 0 to 255)
const int number = 125;

// LED pin assignments in MSB -> LSB order (Bit 7 to Bit 0)
const uint8_t LED_PINS[8] = { 6, 5, 4, 3, 2, 7, 8, 9 };

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

  // Configure all LED pins as OUTPUT using a concise loop
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
}

void loop() {
  // Iterate through all 8 bits from MSB (Bit 7) down to LSB (Bit 0)
  for (uint8_t i = 0; i < 8; i++) {

    // Bitwise shift and mask:
    // 1. (number >> (7 - i)) shifts bit (7 - i) down to position 0
    // 2. & 1 isolates that single bit, evaluating to 1 or 0
    bool bitVal = (number >> (7 - i)) & 1;

    // Drive the corresponding LED (digitalWrite accepts bool directly: true = HIGH, false = LOW)
    digitalWrite(LED_PINS[i], bitVal);

    // Output the bit value to Serial Monitor
    Serial.print(bitVal);
  }

  Serial.println();  // Print newline after full 8-bit output
  delay(1000);       // Pause for 1 second
}