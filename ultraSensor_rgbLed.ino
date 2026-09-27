int trig = 3;
int echo = 2;
const int redPin = 9, greenPin = 10, bluePin = 11;
float distance;

void setup()
{
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  distance = (0.0343 * t) / 2;

  if (distance < 25 || distance == 25)
  {
    setColor(255, 0, 0);
    Serial.println(distance);
    delay(2000);
  }
  else
  {
    setColor(0, 0, 255);
    Serial.println(distance);
    delay(2000);
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, 255 - red);
  analogWrite(greenPin, 255 - green);
  analogWrite(bluePin, 255 - blue);
}