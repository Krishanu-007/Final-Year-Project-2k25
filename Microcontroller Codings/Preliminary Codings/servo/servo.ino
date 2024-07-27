#include <Servo.h>  //in-built library to  control the servo motor

Servo servo;  //creating an object named servo of Servo class
void setup() {
  servo.attach(9);  //defining the pin to which the servo is attached
  Serial.begin(9600);   //Starting the Serial Monitor
}

void loop() {
 servo.write(0);
}
