/*
  An example showing 'ring' analogue meter on a HX8357 TFT
  colour screen
  Needs Fonts 2, 4 and 7 (also Font 6 if using a large size meter)
*/
//https://github.com/Bodmer/TFT_HX8357/blob/master/TFT_HX8357.cpp

// #########################################################################
// Pin Naming
// #########################################################################
#define CSENSE A2
#define HALL 18
#define VSENSE A3

// #########################################################################
// LCD Initialisation
// #########################################################################
#include <TFT_HX8357.h> // Hardware-specific library
TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library
#define HX8357_GREY 0x2104 // Dark grey 16 bit colour
uint32_t runTime = -999;       // time for next update
int reading = 0; // Value to be displayed
int d = 0; // Variable used for the sinewave test waveform
boolean alert = 0;
int8_t ramp = 1;
int xpos = 0, ypos = 0, gap = 0, radius = 0; //Variables for ring meter
int cyclecount = 0;
byte frames;
int fps;
int lastfps;
// #########################################################################
// Hardware Interrupt Initialisation
// #########################################################################
#define FALLING 2 //define falling edge state as "FALLING"
unsigned long start, finished, now; // Variables for time

// #########################################################################
// Variables
// #########################################################################
const int debounce = 10;  // debounce in milliseconds
byte Rotations = 0; //Variable for # of rotations
float Speed = 0; // Variable to hold speed
float LastSpeed; //Variable to hold previous value to clear screen
float MaxSpeed = 0; //Variable for max speed
float LastMaxSpeed; //variable to hold previous value to clear screen

float Voltage ; //Variable to hold voltage
float LastVoltage = 0 ; //variable to hold previous value to clear screen
float Vk = .98; //adjustment for voltage calculation
float Vmin = 100; //Variable that the voltage will be smaller than
float LastVmin; //variable to hold previous value to clear screen

float Current; //variable to hold current
float LastCurrent; //variable to hold previous value to clear screen
float ZeroCurrent = 920; //Current reading when current is 0. 
float Ck = 1; //Adjustment for current calculation
float Amax = 0; //Variable for max current
float LastAmax; //variable to hold previous value to clear screen
int CurrentReading; //variable to hold raw analog input value
int LastCurrentReading;//variable to hold previous value to clear screen

int Power; //Variable to hold speed
int LastPower; //variable to hold previous value to clear screen
int Pmax = 0; //variable to hold max power
int LastPmax; //variable to hold previous value to clear screen

int StartupCycleCount = 50;
// #########################################################################
// Setup
// #########################################################################

void setup(void) {

  //setup LCD
  tft.begin();
  //Serial.begin(9600);
  tft.setRotation(1);
  tft.fillScreen(HX8357_BLACK);
  tft.drawString("Current", 382, 40, 4);
  tft.drawString("Amax", 400, 160, 4);
  tft.drawString("Voltage", 10, 40, 4);
  tft.drawString("Vmin", 10, 160, 4);
  tft.drawCentreString("Speed", 242, 5, 4);
  tft.drawString("MaxSpeed", 345, 260, 4);
  tft.drawCentreString("Power", 242, 140, 4);
  tft.drawString("Pmax", 10, 260, 4);

  //setup hardware interrupt
  pinMode(HALL, INPUT_PULLUP); //set the speedometer switch pin (D18) as an input with an internal pull up to 5V.
  attachInterrupt(digitalPinToInterrupt(18), speedCalc, FALLING); // interrupt called when falling edge is detected on digital pin 18
  start = millis(); //Records the current time as "start"

  pinMode(VSENSE, INPUT);
  pinMode(CSENSE, INPUT);

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 31248;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
  //END TIMER SETUP


  Serial.begin(9600); //initialises serial port at 9600 baud rate
  delay(500); //Waits 3 seconds
}

// #########################################################################
// Main Loop
// #########################################################################

void loop() 
{
  Current = analogRead(CSENSE);
          CurrentReading = Current;
          //Serial.println(Current);
          Current = (ZeroCurrent - Current) * (4.88 / (137216 * 0.0003)) * Ck; //Just is
          //Current = 65;
          if (StartupCycleCount > 1) //Check if it is still in its first few cycles   
            {    
              -- StartupCycleCount; //decrement the variable by one
            }
          
          else if (StartupCycleCount == 0) //execute after the first few cycles
            {   
              if (Current > Amax)
                {
                  Amax = Current ;
                }
            }
            
          else 
            {
              StartupCycleCount = 0;
            }
            
          if (Current < 0.5)
            {
              Current = 0 ;
            }
            
  //if (millis() - runTime >= 0L) 
    { // Execute every 2s
      //runTime = millis();
      //Draw a large meter
      //xpos = 480 / 2 - 160, ypos = 0, gap = 15, radius = 170;
      //reading = Speed;
      //ringMeter(reading, 0 , 60 , xpos, ypos, radius, "KPH", GREEN2RED); // Draw analogue meter
      //Serial.println (Voltage);
      //    frames ++;
      //    tft.setTextColor (TFT_BLACK); //sets text colour in RGB565 format(0x, R, GG, B,)
      //    tft.drawFloat(lastfps, 0, 5 , 20 , 4); // Text, xpos, ypos,
      //    delay (10);
      //    tft.setTextColor (0xF800); //sets text colour in RGB565 format(0x, R, GG, B,)
      //    tft.drawFloat(fps, 0, 5 , 20 , 4); // Text, xpos, ypos,
      //    lastfps = fps;
      if (cyclecount == 100)
        {
          //Voltage = (analogRead((VSENSE)*Vk*60)/1024);
          Voltage = analogRead(VSENSE);
          Voltage = (Voltage * Vk * 60) / 1024;
          cyclecount = 0;
          //Serial.println ("here");
          //Voltage = 48;
          if (Voltage < Vmin)
            {
              Vmin = Voltage ;
            }
    
          tft.setTextColor (TFT_BLACK); //sets text colour in RGB565 format(0x, R, GG, B,)
          tft.drawFloat(LastVoltage, 1, 10 , 80 , 7); // (float, dp, X, Y, font)
          tft.drawFloat(LastVmin, 1, 10 , 190 , 4); // (float, dp, X, Y, font)
          tft.setTextColor (0xB7E0); //sets text colour in RGB565 format(0x, R, GG, B,)
          tft.drawFloat(Voltage, 1, 10 , 80 , 7); // (float, dp, X, Y, font)
          tft.drawFloat(Vmin, 1, 10 , 190 , 4); // (float, dp, X, Y, font)
          LastVoltage = Voltage;
          LastVmin = Vmin;
         
          
            
          tft.setTextColor (TFT_BLACK); //sets text colour in RGB565 format(0x, R, GG, B,)
          tft.drawFloat(LastCurrent, 1, 360 , 80 , 7); // (float, dp, X, Y, font)
          tft.drawFloat(LastAmax, 1, 417 , 190 , 4); // (float, dp, X, Y, font)
          tft.drawNumber(LastCurrentReading, 240 , 290 , 4); // (float, dp, X, Y, font)
          tft.setTextColor (0xB7E0); //sets text colour in RGB565 format(0x, R, GG, B,)
          tft.drawFloat(Current, 1, 360 , 80 , 7); // (float, dp, X, Y, font)
          tft.drawFloat(Amax, 1, 417 , 190 , 4); // (float, dp, X, Y, font)
          tft.drawNumber(CurrentReading, 240 , 290 , 4); // (float, dp, X, Y, font)
          LastCurrent = Current;
          LastAmax = Amax;
          LastCurrentReading = CurrentReading;     
            
          Power = (Current * Voltage);
          if (StartupCycleCount == 0) //execute after the first few cycles
            {   
              if (Power > Pmax)
                {
                  Pmax = Power ;
                }
            }
          tft.setTextColor (TFT_BLACK); //sets text colour in RGB565 format(0x, R, GG, B,)
          tft.drawNumber(LastPower, 130 , 165 , 8); // (float, dp, X, Y, font)
          tft.drawNumber(LastPmax, 10 , 290 , 4); // (float, dp, X, Y, font)
          tft.setTextColor (0xB7E0); //sets text colour in RGB565 format(0x, R, GG, B,)
          tft.drawNumber(Power, 130 , 165 , 8); // (float, dp, X, Y, font)
          tft.drawNumber(Pmax, 10 , 290 , 4); // (float, dp, X, Y, font)
          LastPower = Power;
          LastPmax = Pmax;
    
          //Speed = 50;
          
          //if (Speed <=2)
            //{
             // Speed = 0;
            //}
          if (Speed > MaxSpeed)
            {
              MaxSpeed = Speed ;
            }
          tft.setTextColor (TFT_BLACK); //sets text colour in RGB565 format(0x, R, GG, B,)
          tft.drawFloat(LastSpeed, 1, 145 , 35 , 8); // (float, dp, X, Y, font)
          tft.drawFloat(LastMaxSpeed, 1, 417 , 290 , 4); // (float, dp, X, Y, font)
          delay (10);
          tft.setTextColor (0xB7E0); //sets text colour in RGB565 format(0x, R, GG, B,)
          tft.drawFloat(Speed, 1, 145 , 35 , 8); // (float, dp, X, Y, font)
          tft.drawFloat(MaxSpeed, 1, 417 , 290 , 4); // (float, dp, X, Y, font)
          LastSpeed = Speed;
          LastMaxSpeed = MaxSpeed;
        }  
      
      else
        {
          cyclecount ++; //increase cyclecount by one
        } 
      
    }
}



void speedCalc()
{
  now = millis();
  if (now - start > debounce)
  {
    Rotations++ ;
    //Serial.println (Rotations);
    start = now;
  }
}

ISR(TIMER1_COMPA_vect)
{
  Speed = Rotations;
  //Serial.println (Speed);
  //Speed = (0.88*7.2*Speed);
  Rotations = 0;
  fps = frames;
  frames = 0;
}
