/* 
 * =====================================================================================
 *  THEORETICAL CONCEPTS: PIR (Passive) vs. Active IR Sensors
 * =====================================================================================
 * 
 * 1. PASSIVE INFRARED (PIR) SENSOR:
 *    - Operating Principle : Detects changes in ambient heat radiation emitted naturally
 *                            by warm bodies (humans, pets).
 *    - Emission            : PASSIVE — Has NO transmitter/IR LED. Strictly receives energy.
 *    - Primary Purpose     : Motion detection over a broad area.
 *    - FOV & Range         : Wide field of view (~110 degrees), long distance (up to 7 meters).
 *    - Power Consumption   : Extremely low (microamps) because no light source is powered.
 *    - Common Applications : Intruder alarms, automatic room lighting, occupancy sensing.
 * 
 * 2. ACTIVE INFRARED (IR) SENSOR:
 *    - Operating Principle : Emits IR light via an IR LED and measures the light bounce-back
 *                            reflected off an object using a photodiode/phototransistor.
 *    - Emission            : ACTIVE — Continuously transmits IR light.
 *    - Primary Purpose     : Proximity sensing, obstacle detection, or line tracking.
 *    - FOV & Range         : Narrow straight line, short distance (typically 2 cm - 30 cm).
 *    - Power Consumption   : Higher than PIR due to continuously driving the IR transmitter LED.
 *    - Common Applications : Line-following robots, hand sanitizer dispensers, conveyor belts.
 * =====================================================================================
 */ 

const int PIR_PIN = 2;       // PIR sensor output connected to Digital Pin 2
const int LED_PIN = 13;      // Onboard LED pin

void setup() {
  pinMode(PIR_PIN, INPUT);   // PIR pin acts as digital INPUT (Receives HIGH or LOW)
  pinMode(LED_PIN, OUTPUT);  
  Serial.begin(9600);
  
  // PIR sensors require a warm-up/calibration phase (30-60s) to learn ambient room heat
  Serial.println("PIR Sensor Calibrating... Please wait.");
  delay(30000); 
  Serial.println("PIR Active!");
}

void loop() {
  /*
   * PIR SIGNAL READING:
   * Returns HIGH (3.3V) when a warm moving body enters its broad FOV.
   * Returns LOW  (0V)  when idle or no dynamic heat variation is detected.
   */
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("[PIR ALERT] Heat motion detected in wide zone!");
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(200); // Sampling delay
}