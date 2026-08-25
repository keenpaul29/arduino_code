/*
  ==============================================================================
  THEORY & WORKING PRINCIPLE: SOIL MOISTURE SENSING WITH ARDUINO
  ==============================================================================
  1. ANALOG-TO-DIGITAL CONVERSION (ADC):
     - The ATmega328P microcontroller on the Arduino Uno contains a 10-bit 
       Successive Approximation Analog-to-Digital Converter (ADC).
     - It maps an incoming analog voltage (0V to 5V reference) onto a discrete 
       digital scale ranging from 0 to 1023 (2^10 = 1024 unique levels).
     - Formula: ADC Value = (Input Voltage / Reference Voltage) * 1023
     - Resolution: 5.0V / 1024 ≈ 4.88 mV per step.

  2. SOIL MOISTURE SENSING TYPES:
     A. Resistive Soil Moisture Sensors:
        - Measure electrical resistance between two exposed probes inserted in soil.
        - Water contains dissolved ions, making it conductive.
        - High soil moisture  => Low resistance => Higher voltage signal => Higher ADC value (~500–1023).
        - Low/Dry soil        => High resistance => Lower voltage signal => Lower ADC value (~0–499).
        - Disadvantage: Prone to corrosion over time due to electrolysis.

     B. Capacitive Soil Moisture Sensors:
        - Measure changes in electrical capacitance caused by the soil dielectric constant.
        - Water has a high dielectric constant (~80) compared to dry soil (~3–5).
        - Capacitive sensors typically work in reverse: High ADC values mean dry soil, 
          while low ADC values mean wet soil.

  3. SERIAL COMMUNICATION (UART):
     - Uses Universal Asynchronous Receiver-Transmitter (UART) protocol on digital pins 0 & 1.
     - Transmits real-time ADC readings over USB to a host PC at a defined baud rate 
       (bits per second) for visual logging and threshold calibration.
  ==============================================================================
*/

// Assign analog input pin A0 to sample the continuous voltage signal from the sensor module
const int sensor_pin = A0; 
int threshold = 500;

void setup() {
    // Initialize UART hardware serial communication at a standard rate of 9600 baud (bits/sec).
    // This allows data transmission between the Arduino and the Arduino IDE Serial Monitor.
    Serial.begin(9600);
    
    // Explicitly configure pin A0 as an INPUT.
    // Configures the pin's internal circuitry to high-impedance mode, drawing negligible current.
    pinMode(sensor_pin, INPUT);
}

void loop() {
    // Read the analog voltage level on pin A0 via the internal ADC.
    // Returns an integer between 0 (representing 0V) and 1023 (representing 5V).
    int moistureValue = analogRead(sensor_pin);
    
    // Transmit the digitized soil moisture value over the serial bus, 
    // followed by a carriage return and line feed (\r\n) for readability.
    if (moistureValue > threshold) {
        Serial.print(moistureValue);
        Serial.println("Soil is dry!");

    }
    else {
        Serial.print(moistureValue);
        Serial.println("Soil is wet!");
    }
    
    // Introduce a short execution delay (200 milliseconds) between readings.
    // Prevents overwhelming the Serial Monitor buffer and stabilizes ADC sampling cycles.
    delay(200);
}