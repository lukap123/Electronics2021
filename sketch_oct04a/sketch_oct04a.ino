@@ -46,8 +46,12 @@ const int debounce = 10;  // debounce in milliseconds
byte Rotations = 0; //Variable for # of rotations
byte Speed = 0; // Variable to hold speed
float Voltage ; //Variable to hold voltage
float LastVoltage = 0 ;
float Vk = .98;
float LastVoltage = 0 ; //variable to hold previous value to clear screen
float Vk = .98; //adjustment for voltage calculation
float Current; //variable to hold current
float LastCurrent; //variable to hold previous value to clear screen
float ZeroCurrent = 929; //Current reading when current is 0.
float Ck = 1; //Adjustment for current calculation
// #########################################################################
// Setup
// #########################################################################
@ -65,6 +69,7 @@ void setup(void) {
  start = millis(); //Records the current time as "start"

  pinMode(VSENSE, INPUT);
  pinMode(CSENSE, INPUT);

  cli();//stop interrupts

@ -99,14 +104,14 @@ void loop() {
    xpos = 480 / 2 - 160, ypos = 0, gap = 15, radius = 170;
    reading = Speed;
    ringMeter(reading, 0 , 60 , xpos, ypos, radius, "KPH", GREEN2RED); // Draw analogue meter
    Serial.println (Voltage);
    //Serial.println (Voltage);
    frames ++;
    tft.setTextColor (TFT_BLACK); //sets text colour in RGB565 format(0x, R, GG, B,)
    tft.drawFloat(lastfps, 2, 0 , 20 , 4); // Text, xpos, ypos,
    tft.setTextColor (0xF800); //sets text colour in RGB565 format(0x, R, GG, B,)
    tft.drawFloat(fps, 2, 0 , 20 , 4); // Text, xpos, ypos,
    lastfps = fps;
    if (cyclecount == 5)
    if (cyclecount == 1)
    {
      //Voltage = (analogRead((VSENSE)*Vk*60)/1024);
      Voltage = analogRead(VSENSE);
@ -118,6 +123,15 @@ void loop() {
      tft.setTextColor (0xF800); //sets text colour in RGB565 format(0x, R, GG, B,)
      tft.drawFloat(Voltage, 1, 0 , 300 , 4); // Text, xpos, ypos,
      LastVoltage = Voltage;
      Current = analogRead(CSENSE);
      Serial.println(Current);
      Current = (ZeroCurrent-Current)*(5/(137216*0.0003))*Ck;
      tft.setTextColor (TFT_BLACK); //sets text colour in RGB565 format(0x, R, GG, B,)
      tft.drawFloat(LastCurrent, 1, 0 , 150 , 4); // Text, xpos, ypos,
      tft.setTextColor (0xF800); //sets text colour in RGB565 format(0x, R, GG, B,)
      tft.drawFloat(Current, 1, 0 , 150 , 4); // Text, xpos, ypos,
      LastCurrent = Current;
      
    }
    else
    {
@ @@
