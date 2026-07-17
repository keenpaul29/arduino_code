// HC-SR04 Ultrasonic Distance Sensor sensing distance with red and blue LEDs

int redLight = 4;
int blueLight = 2;
int trig = 12;
int echo = 11;
float distance = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redLight, OUTPUT);
  pinMode(blueLight, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

float MicroSensor(int trigger, int echo) {
  digitalWrite(trigger, LOW);
  delayMicroseconds(10);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigger, LOW);
  long t = pulseIn(echo, HIGH);
  float dist = (0.0343 * t) / 2;
  Serial.println(dist);
  return (dist==0)? MicroSensor(trigger,echo): dist;  //ternary + recursively used func to avoid printing null value
  
}

void loop() {
  // put your main code here, to run repeatedly:
  float distance = MicroSensor(trig, echo);
  if (distance < 30 || distance == 30) {
      digitalWrite(redLight, LOW);
      digitalWrite(blueLight, HIGH);
  }
  else {
      digitalWrite(blueLight, LOW);
      digitalWrite(redLight, HIGH);
  }
  delay(50);
}