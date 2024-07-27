#include <NewPing.h>  //Using inbuilt library to reduce calculations
int trig=2;   //defining the trigger and echo pins
int echo=4;

NewPing sensor(trig,echo,150);  //creating a object named 'sensor' under NewPing class
void setup() {
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned int dist_cm=sensor.ping_cm();  //ping_cm() function is used to get the distance in centimeters [can use ping_in() for inches or ping_median(i) for taking a median of i values ]
  Serial.print("The distance is: ");
  Serial.println(dist_cm);
  double time=sensor.ping();  //taking the ping() to calculate the time for taking the distance measure.
  Serial.print("The time taken is: ");
  Serial.println(time);
  delay(100);
}
