#include<Servo.h>
Servo s;
int servopin=9;
void setup() {
  // put your setup code here, to run once:
  s.attach(servopin);
}

void loop() {
  // put your main code here, to run repeatedly:
 for(int deg=180;deg>=0;deg--) {
  s.write(deg);
  delay(50);
 }

}