/* ============================================================================
   PROJECT: Servo Motor Controlled Sweep
   
   HARDWARE COMPONENTS:
   1. Arduino Board (Microcontroller / Brain):
      - Handles execution, hardware timers, and PWM signal generation.
   2. Servo Motor (Electro-mechanical Actuator):
      - Internal DC Motor: Provides mechanical force (torque).
      - Control Board / H-Bridge: Decodes incoming PWM pulses and drives motor.
      - Potentiometer: Feedback sensor attached to output shaft to measure current angle.
      - Gear Train: Reduces DC motor RPM to increase rotational torque.
   3. Wiring / Connections:
      - Orange/Yellow/White Wire -> Digital Pin 9 (PWM Control Signal)
      - Red Wire                 -> 5V Power (External 5V-6V source if under load)
      - Brown/Black Wire         -> GND (Must share Common Ground with Arduino)
   ============================================================================ */

#include <Servo.h> 
// <Servo.h> LIBRARY (Software Component):
// Acts as an abstraction layer over low-level hardware timers (e.g., Timer1 on Uno).
// Converts angles (0-180°) into exact microsecond pulse widths (1ms = 0°, 2ms = 180°)
// repeated at a 50Hz frequency (20ms period) without blocking main code execution.

Servo s; 
// CLASS INSTANCE (Software Component):
// Instantiates a Servo object named 's' in memory to manage state and pulse timing.

int servopin = 9; 
// SIGNAL PIN DEFINITION (Hardware Pin Selection):
// Digital Pin 9 supports hardware PWM output required for precise pulse control.

void setup() {
  // s.attach() METHOD:
  // Binds the Servo object 's' to Pin 9, sets Pin 9 to OUTPUT mode, and initializes 
  // the underlying hardware timer to start emitting 50Hz control signals.
  s.attach(servopin);
}

void loop() {
  // DECREMENTING FOR LOOP:
  // Sweeps angle backwards from 180° down to 0° in 1° steps.
  for(int deg = 180; deg >= 0; deg--) {
    
    // s.write() METHOD:
    // Translates 'deg' into corresponding pulse width in microseconds, updating 
    // the timer registers. The servo's internal board compares this with its internal 
    // potentiometer feedback to rotate the DC motor to the target angle.
    s.write(deg);
    
    // delay(50) BLOCKING PAUSE:
    // Halts program execution for 50 milliseconds per step. Gives the physical 
    // motor and gear train enough time to physically move to the new position.
    // Full sweep duration: 181 steps * 50ms = 9.05 seconds.
    delay(50);
  }
  
  // NOTE ON LOOP REPEAT & JUMP:
  // Once 'deg' reaches 0, loop() finishes and instantly restarts at deg = 180.
  // This causes the servo shaft to jump rapidly back to 180° before starting 
  // the next slow sweep down to 0°.
}