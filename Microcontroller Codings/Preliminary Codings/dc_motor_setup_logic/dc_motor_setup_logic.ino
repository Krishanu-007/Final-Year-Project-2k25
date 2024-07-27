/*===================================
 TO GOVERN ANY DC MOTOR WITH TWO CONTROL PINS
 THE FOLLOWING PATTERN IS TO BE REMEMBERD:
 
 LET THE TWO CONTROL PINS BE A,B
 
 THEN THE CONTROL OF THE DC MOTOR BY THESE PINS WILL BE:
 
 	  A			     B			    MOTION
   =====	   =====       ========
   LOW			  LOW		      NO MOTION
   LOW 			  HIGH	      CLOCKWISE
   HIGH			  LOW		      ANTICLOCKWISE
   
 HERE THE CONTROL PINS ARE i1,i2,i3,i4
 
 =====================================    */

#define i1 2	//i1,i2 governs one motor
#define i2 3	
#define i3 4	//i3,i4 governs one motor
#define i4 5

void setup()
{
  pinMode(i1,OUTPUT); //Setting up the control pins for each motor
  pinMode(i2,OUTPUT);
  pinMode(i3,OUTPUT);
  pinMode(i4,OUTPUT);
}

void loop()
{
  //forward
  digitalWrite(i1,LOW);
  digitalWrite(i2,HIGH);
  digitalWrite(i3,LOW);
  digitalWrite(i4,HIGH);
  delay(2000);
  
 //backward
  digitalWrite(i1,HIGH);
  digitalWrite(i2,LOW);
  digitalWrite(i3,HIGH);
  digitalWrite(i4,LOW);
  delay(2000);
  
  //left-turn
  digitalWrite(i1,LOW);
  digitalWrite(i2,LOW);
  digitalWrite(i3,LOW);
  digitalWrite(i4,HIGH);
  delay(2000);
  
  //right-turn
  digitalWrite(i1,LOW);
  digitalWrite(i2,HIGH);
  digitalWrite(i3,LOW);
  digitalWrite(i4,LOW);
  delay(2000);
}