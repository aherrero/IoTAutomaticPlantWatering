#include <LowPower.h>       //Library to sleep the micro

/*
 * Input/Output
 */

int inMoistureSensor = 0;    //Analog: Read Sensor
int pinOnMoistureSensor = 2; //Digital: on/off sensor
int outControl = 3;          //PWM Digital: Motor output
int pinInternalLed = 13;     //Digital: Led output (internal) used as indicator

/*
 * Var
 */
 
int motorSpeed = 255;

/*
 * Functions
 */
 
int getMoistureValue()
{
  // sensor temperature
  int sensorMoisture = analogRead(inMoistureSensor);  
  //Serial.print(sensorMoisture); Serial.println(" raw value.");
  return sensorMoisture;
}
 
/*
 * setup()
 */
void setup()
{
  Serial.begin(9600);                   //Start the serial connection with the computer
  
  pinMode(pinOnMoistureSensor, OUTPUT); //pin to power on the sensor
  pinMode(outControl, OUTPUT);          //pin to control de pump motor
  
  pinMode(pinInternalLed, OUTPUT);      //led to show if the pump motor is working
}


/*
 * loop()
 */
void loop()                     // run over and over again
{  
  //Show the micro wakes up
  digitalWrite(pinInternalLed, HIGH);
  
  //Power on the moisture Sensor
  digitalWrite(pinOnMoistureSensor, HIGH);
  delay(100);
    
  //Read Sensor
  int sensorMoisture = getMoistureValue();
  
  //Action
  int firstAction = 0;
  while(sensorMoisture < 800)   //emperic value to determinate if the plant needs water
  {
    //Notify the pump motor is running with blink
    digitalWrite(pinInternalLed, LOW);
    delay(100);
    
    //Run pump motor
    if(firstAction == 0)
    {
      analogWrite(outControl, motorSpeed);
      firstAction = 1;
    }
    
    //Update sensor value
    sensorMoisture = getMoistureValue();
    
    //Blink
    digitalWrite(pinInternalLed, HIGH);
    delay(100);
  }
  
  //Power off pump motor
  analogWrite(outControl, 0);
  
  //Show the micro is going to wait
  digitalWrite(pinInternalLed, LOW);
  
  //Power off the moisture Sensor
  digitalWrite(pinOnMoistureSensor, LOW);
  delay(100);

  //Main Sleep
  //delay(2000);
  
  //Sleep 24 hours (3600 seconds)
  int endForSleep = 3600/8;  //8 -> 8 sec is the max to put in the LowPower library to sleep
  for(int i=0; i<endForSleep; i++)
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
