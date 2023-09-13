 #define BUZZER 3            // define Buzzer pin
 #define LATCH  4            // define Latch led pin
 #define LAMP   5            // define Lamp led pin
 #define CLK    7            // define Clk led pin
 #define DATA   8            // define Data led pin

 #define POT_PIN   A0            // define Potentiometer pin
 #define DOOR_SW   A1            // define Door button pin
 #define BTN_STOP  A3            // define Start button pin
 #define BTN_START A2            // define Stop button pin

void pciSetup(byte pin); // setup what will be an interrupt
ISR(PCINT1_vect);        //Interupt Service Rutine is called when interrupt value changes

void setPinModes() 
{
  pinMode(DATA,  OUTPUT);        // set DATA  lamp to output
  pinMode(CLK,   OUTPUT);        // set CLK   lamp to output
  pinMode(LATCH, OUTPUT);        // set LATCH lamp to output
  pinMode(LAMP,  OUTPUT);        // set LAMP  lamp to output
  pinMode(BUZZER,  OUTPUT);      // set BUZZER     to output
  pinMode(BTN_START, INPUT);
  pinMode(BTN_STOP,  INPUT);
  pinMode(DOOR_SW,   INPUT);
  pinMode(POT_PIN,   INPUT);


  pciSetup(BTN_START);  //set START button as an interrupt
  pciSetup(BTN_STOP);   //set STOP  button as an interrupt
  pciSetup(DOOR_SW);    //set STOP  button as an interrupt

  digitalWrite(BUZZER, LOW);

}

ISR(PCINT1_vect)
{
  int startState = digitalRead(BTN_START);       //read Start button
  static int lastStartState;
  if (!startState && lastStartState) callbackBtn(BTN_START, startState);
  lastStartState = startState;
  
  int stopState = digitalRead(BTN_STOP);
  static int lastStopState; 
  if (!stopState && lastStopState)   callbackBtn(BTN_STOP, stopState);
  lastStopState = stopState;

  int doorState = digitalRead(DOOR_SW);
  static int lastDoorState;
  if(doorState != lastDoorState)     callbackBtn(DOOR_SW, doorState);
  lastDoorState = doorState;
    
}

void pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= 1 << digitalPinToPCMSKbit(pin);

  PCIFR |= 1 << digitalPinToPCICRbit(pin);
 
  PCICR |= 1 << digitalPinToPCICRbit(pin);
}
