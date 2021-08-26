#define CHANGE 1
#define FALLING 2
#define RISING 3

unsigned long start, finished;
const int debounce = 200;  // debounce in milliseconds
unsigned long now = 0;

byte Rotations = 0;

void setup()
{
  pinMode(18, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(18), speedCalc, FALLING); // interrupt called when falling edge is detected on digital pin 18
  start = millis();
  delay(3000);
  Serial.begin(9600);

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


void speedCalc()
{
  now = millis();
  if (now - start > debounce)
  {
    Rotations++ ;
    start = now;
  }
}

ISR(TIMER1_COMPA_vect) 
{
  Serial.println(Rotations);
  Rotations = 0;
}




void loop()
{

  delay (500);

}
