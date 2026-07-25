/* 
 * =====================================================================================
 *  THEORETICAL CONCEPTS: PIR Sensor + Buzzer + Relay Control
 * =====================================================================================
 * 
 * 1. ACTIVE BUZZER CONTROL:
 *    - Has an internal oscillator. Driving its signal pin HIGH creates a constant tone.
 *    - Used for instant auditory feedback upon sensor state transitions.
 * 
 * 2. RELAY MODULE OPERATION:
 *    - Allows a low-power 5V micro-controller to safely toggle high-voltage loads.
 *    - MOST relay modules are ACTIVE LOW (IN = LOW turns relay ON, IN = HIGH turns OFF).
 *    - Always verify whether your relay module triggers on HIGH or LOW logic.
 * 
 * 3. SENSOR CALIBRATION (LATCHING DELAY):
 *    - PIR sensors need ~30s on boot to measure static infrared baseline of the room.
 * =====================================================================================
 */

const int PIR_PIN   = 2; // Input: PIR Motion Sensor Output
const int RELAY_PIN = 7; // Output: Controls Relay Switch (Lamp/Siren)
const int BUZZER_PIN = 8; // Output: Controls Active Piezo Buzzer

// Set based on your relay module type (Active LOW vs Active HIGH)
const int RELAY_ON  = LOW;  
const int RELAY_OFF = HIGH; 

void setup() {
  pinMode(PIR_PIN, INPUT); 
  pinMode(RELAY_PIN, OUTPUT); 
  pinMode(BUZZER_PIN, OUTPUT); 

  // Initialize outputs in safe OFF state on boot
  digitalWrite(RELAY_PIN, RELAY_OFF); 
  digitalWrite(BUZZER_PIN, LOW); 

  Serial.begin(9600); 
  
  // Calibration period to prevent false alarms on system start
  Serial.println("Calibrating PIR sensor baseline... Stay clear.");
  delay(30000); 
  Serial.println("System Armed & Ready!");
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    // 1. Activate Visual/Power Load via Relay
    digitalWrite(RELAY_PIN, RELAY_ON);
    
    // 2. Sound the Alert Buzzer
    digitalWrite(BUZZER_PIN, HIGH);
    
    Serial.println("[ALARM] Motion Detected! Relay Triggered & Buzzer Sounding!");
  } else {
    // Deactivate Alarm components when area is clear
    digitalWrite(RELAY_PIN, RELAY_OFF);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(200); // Polling stability delay
}