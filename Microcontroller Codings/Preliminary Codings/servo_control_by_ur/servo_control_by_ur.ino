/*=========================================
THIS PROGRAM IS DESIGNED TO CONTROL THE MOTION OF THE SERVO MOTOR
BASED ON THE DIRECTION DETECTED BY THE ULTRASONIC SENSOR WITHIN A
A RANGE.
THE MAIN MOTIVE IS TO STUDY THE DIRECTIVITY OF THE SERVO CONTROLLED
BY THE UR SENSOR.
=============================================   */
#include <Servo.h>/ //including the library
#define Max 35   //defining the limits of detection by the ultrasonic sensor
#define Mid 25
#define Min 15
int trig=2; //defining the trigger and echo pins
int echo=3;
int pos=90; //defining the position of the servo at idle condition and the maximum extent upto which it rotates on each side(+-45)
int max_pos_right=150;
int max_pos_left=45;
Servo motor;  //creating object named motor of Servo class
void setup() {
  pinMode(trig,OUTPUT); //setting the modes of the pins and setting the serial monitor
  pinMode(echo,INPUT);
  Serial.begin(9600);
  motor.attach(9);  //setting the servo signal pin to pin 9(digital)
  motor.write(pos); //setting the servo to 90 degree at idle contion
}

void loop() {
  unsigned int dist=dist_cm();  //calculating the distance
  if(dist>=Mid && dist<=Max)  //condition to move on the right side 
  {
    for(int i=pos;i<=max_pos_right;i++) 
    {
      motor.write(i); //moving the servo motor by 1 degree towards maximum right as defined
      delay(15);
    }
  delay(1000);  //waiting for 1s at that postion(max)
   for(int i=max_pos_right;i>=pos;i--)  //return to original position
    {
      motor.write(i);
      delay(15);
    }
    delay(1000);
  }
  else if(dist<Mid && dist>=Min)  //condition to move on the left side
  {
    for(int i=pos;i>=max_pos_left;i--)
    {
      motor.write(i);
      delay(15);
    }
    delay(1000);
    for(int i=max_pos_left;i<=pos;i++)
    {
      motor.write(i);
      delay(15);
    }
    delay(1000);
  }
  else
  {
    motor.write(90);  //condition for other cases causing to bring back to the idle position 
  }
}
unsigned int dist_cm()  //function to calculate distance using brute force
{
  digitalWrite(trig,LOW); 
  delayMicroseconds(2); 
  digitalWrite(trig,HIGH);  
  delayMicroseconds(10);    
  digitalWrite(trig,LOW);   
  long time=pulseIn(echo,HIGH); 
  unsigned int distance=(time*0.034)/2;
  return distance;
}