// HW-201 IR Infrared Obstacle Avoidance Sensor Module

int ir=8; // digital pin of infrared sensor
int l1=2; // digital pin no. of first bulb
int l2=4; // digital pin no. of second bulb
void setup() {
  // put your setup code here, to run once:
  pinMode(l1,OUTPUT);
  pinMode(l2,OUTPUT);
  pinMode(ir,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value=digitalRead(ir);
  if(value==1)
  {
    digitalWrite(l1,1);
    digitalWrite(l2,0);
  }
  else
  {
    digitalWrite(l1,0);
    digitalWrite(l2,1);
  }
}