int trig = 9;
int echo = 8;
float distance = 0;

void setup() {
  Serial.begin(9600);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {

  // ------------------- Trigger the HC-SR04 Sensor -------------------
  // The ultrasonic sensor does NOT measure distance continuously.
  // We must send a trigger pulse to tell it to start a measurement.
  //
  // Step 1: Keep the TRIG pin LOW for 2 microseconds.
  // This stabilizes the sensor and ensures any previous trigger signal
  // has completely ended before starting a new measurement.
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // Step 2: Make the TRIG pin HIGH for 10 microseconds.
  // A HIGH pulse of at least 10 µs tells the HC-SR04 to:
  //   1. Transmit 8 ultrasonic sound waves (40 kHz)
  //   2. Wait for those waves to bounce back from an object
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  // Step 3: Bring the TRIG pin LOW again.
  // The trigger pulse is complete, and the sensor now starts
  // listening for the reflected ultrasonic wave.
  digitalWrite(trig, LOW);

  // Measure how long the ECHO pin stays HIGH.
  // The ECHO pin becomes HIGH when the sound is transmitted
  // and goes LOW after the reflected sound is received.
  // Therefore, 't' is the total travel time of the sound wave.
  long t = pulseIn(echo, HIGH);

  // Calculate the distance.
  // Speed of sound = 0.0343 cm per microsecond.
  //
  // Distance traveled by sound = Speed × Time
  //
  // Since the sound travels:
  //   Sensor ---> Object ---> Sensor
  //
  // the measured time represents a ROUND TRIP.
  // Therefore, divide by 2 to obtain the one-way distance.
  distance = (0.0343 * t) / 2;

  // Print the calculated distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Wait 100 ms before taking the next measurement
  delay(100);
}