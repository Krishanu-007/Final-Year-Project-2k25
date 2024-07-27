/*==================================================
 THIS PROGRAM IS DESIGNED TO CONTROL THE DC MOTOR ROTATION 
 USING ULTRASONIC SENSOR.

 HERE WE HAVE TAKEN THREE CASES OF MOTION i.e. FORWARD,BACKWARD AND STOP
 THE CLASSIFICATION OF THE MOTIONS ARE DONE ON THE BASIS OF
 THE DISTANCE MEASURED BY THE UR SENSOR.

HERE WE HAVE TAKEN THE DIRECTIONS FOR FORWARD AND BACKWARD TO BE ARBITARY
OUR MAIN MOTIVE IS TO CHANGE THE DIRECTION WITH RESPECT TO THE DISTANCE MEASURED
 ========================================================== */

#define max 150 //Setting the detection ranges to be 150, 100, 50 cms
#define mid 100 
#define min 50
int c1=4; //c1 and c2 are the control pins of the dc motor
int c2=5;
int trig=2; //defining the trigger and echo pins for ultrasonic detector
int echo=3;
void setup() {
  pinMode(c1,OUTPUT);   //Setting up the pinmode of the pins and setting up the serial monitor
  pinMode(c2,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long dist=dist_cm(); //calculating the distance
  if(dist>=mid && dist<=max)  //condition to move forward
  {
    forward();  //calling the forward function
    Serial.println("forward");
  }
  else if(dist>=min && dist<mid)  //condition to move backward
  {
    backward(); //calling the backward function
    Serial.println("backward");
  }
  else
  {
    stop(); //calling the stop function for other cases
    Serial.println("stop");
  }
}
unsigned int dist_cm()  //function to calculate the distance(brute force) 
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
void forward()  //forward function
{
  digitalWrite(c1,LOW);
  digitalWrite(c2,HIGH);
}
void backward() //backward function
{
  digitalWrite(c1,HIGH);
  digitalWrite(c2,LOW);
}
void stop() //stop function
{
  digitalWrite(c1,LOW);
  digitalWrite(c2,LOW);
}