#define CHANGE 1
#define FALLING 2
#define RISING 3

unsigned long start, finished;
const int debounce = 200;  // debounce in milliseconds
unsigned long now = 0;

byte Rotations = 0;

//

void setup()
{
  pinMode(18, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(18), speedCalc, FALLING); // interrupt called when falling edge is detected on digital pin 18
  start = millis();
  delay(3000);
  Serial.begin(9600);
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

void loop()
{

  delay (500);

}
