/* TMP36 analog temperature sensor with Arduino example code. More info: https://www.makerguides.com */


//libaries
//Include libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 6 on the Arduino
#define ONE_WIRE_BUS 12
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Define to which pin of the Arduino the output of the TMP36 is connected:
#define sensorPin A4
#define reed A3//pin connected to read switch



//storage variables
float radius = 13.5;// tire radius (in inches)- CHANGE THIS FOR YOUR OWN BIKE

int reedVal;
long timer = 0;// time between one full rotation (in ms)
float mph = 0.00;
float circumference;

int maxReedCounter = 100;//min time (in ms) of one rotation (for debouncing)
int reedCounter;

int randit69 = 0;

float vTuning = 14.6;

void setup() {
    reedCounter = maxReedCounter;
  circumference = 2*3.14*radius;
  pinMode(1,OUTPUT);//tx
  pinMode(reed, INPUT);
  sensors.begin();
 
  
  Serial.write(12);//clear
  
  // TIMER SETUP- the timer interrupt allows preceise timed measurements of the reed switch
  //for mor info about configuration of arduino timers see http://arduino.cc/playground/Code/Timer1
  cli();//stop interrupts

  //set timer1 interrupt at 1kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;
  // set timer count for 1khz increments
  OCR1A = 1999;// = (1/1000) / ((1/(16*10^6))*8) - 1
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts
  //END TIMER SETUP
  
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect) {//Interrupt at freq of 1kHz to measure reed switch
  reedVal = digitalRead(reed);//get val of A0
  if (reedVal){//if reed switch is closed
    if (reedCounter == 0){//min time between pulses has passed
      mph = (56.8*float(circumference))/float(timer);//calculate miles per hour
      timer = 0;//reset timer
      reedCounter = maxReedCounter;//reset reedCounter
    } 
    else{
      if (reedCounter > 0){//don't let reedCounter go negative
        reedCounter -= 1;//decrement reedCounter
      }
    }
  }
  else{//if reed switch is open
    if (reedCounter > 0){//don't let reedCounter go negative
      reedCounter -= 1;//decrement reedCounter
    }
  }
  if (timer > 2000){
    mph = 0;//if no new pulses from reed switch- tire is still, set mph to 0
  }
  else{
    timer += 1;//increment timer
  } 
}



void loop() {

  float Voltage = analogRead(A0);
  Voltage = Voltage/vTuning;

  // Get a reading from the temperature sensor:
  int reading = analogRead(sensorPin);

  // Convert the reading into voltage:
  float voltage = reading * (5000 / 1024.0);

  // Convert the voltage into the temperature in Celsius:
  float temperature = (voltage - 500) / 10;

  float ZeroCurrentV = 5*analogRead(A1)/1024;
  float Current = 25*((5)*analogRead(A1)/1024 - ZeroCurrentV);

  Serial.print(Voltage);
  Serial.print("V");   
  Serial.print("|");
  Serial.print(Current);
  //Serial.print(cCurrent);
  Serial.print("A");
  Serial.print("|");
  Serial.print(mph * 1.61);
  Serial.print("km/h");
  Serial.print("|");
  Serial.print("23.52");
  Serial.println("C");
  //Serial.print("|");
  //Serial.println(randit69);
  delay(600);
   
}
