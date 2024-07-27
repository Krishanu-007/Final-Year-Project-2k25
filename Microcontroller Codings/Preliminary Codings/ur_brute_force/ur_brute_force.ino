#define Max 200 //defining the maximum range for the sensor to detect
#define Min 10  //defining the minimum range for the sensor to detect
int trig=2;     /*Defining the trigger and echo pin numbers
int echo=4;     */
void setup() {
  pinMode(trig,OUTPUT); //Setting the pinmodes of each trigger and echo pins
  pinMode(echo,INPUT);
  Serial.begin(9600);     //also setting the baud rate of the serial monitor for communication*/

}
void loop() {
  delay(100);
  digitalWrite(trig,LOW); //clearing the trigger pin from any previous data
  delayMicroseconds(2); 
  digitalWrite(trig,HIGH);  //turning the trigger pin ON to send signal
  delayMicroseconds(10);    //duration of sending the signal
  digitalWrite(trig,LOW);   //turning the trigger pin OFF
  long time=pulseIn(echo,HIGH); //calculating the time for the echo pin to receive high signal
  unsigned long distance=(time*0.034)/2;  //calculating the distance
  if(distance>Max || distance<Min)  //condition for limiting the detection of the sensor.
  {
    distance=0;
  }
  Serial.print("The distance is: ");
  Serial.println(distance);
}
