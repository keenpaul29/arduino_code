int redPin = 3;
int bluePin = 9;
void setup() {
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(bluePin, HIGH);
  digitalWrite(redPin, LOW);
  delay(1000);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, HIGH);
  delay(1000);
}