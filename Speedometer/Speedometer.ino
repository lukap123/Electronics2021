// #########################################################################
// Pin Naming
// #########################################################################
#define HALL 18

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


void setup() {

  //setup hardware interrupt
  pinMode(HALL, INPUT_PULLUP); //set the speedometer switch pin (D18) as an input with an internal pull up to 5V.
  attachInterrupt(digitalPinToInterrupt(18), speedCalc, RISING); // interrupt called when falling edge is detected on digital pin 18
  start = millis(); //Records the current time as "start"

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
  //END TIMER SETUP
}

void loop() {
  // put your main code here, to run repeatedly:

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
  Rotations = 0;
  fps = frames;
  frames = 0;
}
