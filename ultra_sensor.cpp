int trig=9;
int echo=8;
float distance=0;
void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600); //
   pinMode(trig,OUTPUT);
   pinMode(echo,INPUT);
   // Serial.println("hello");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trig,LOW);
  delayMicroseconds(10);
  digitalWrite(trig,HIGH);
  delayMicroseconds(2);
  digitalWrite(trig,LOW);
  long t=pulseIn(echo,HIGH); //
  distance=(0.0343*t)/2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);

}