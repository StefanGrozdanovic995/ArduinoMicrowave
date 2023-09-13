// ------- Prototype Functions -------
void tick();
int callbackKeypad(int key);
void callbackBtn(int btnNo, int state);

#include "IOSetup.h"
#include "MCP.h"
#include "Keypad.h"
#include "Display.h"
#include "Cooker.h"
#include "Lamp.h"
#include "PowerPot.h"

#include <TimerOne.h>


// -------- SETUP ------------- 
void setup() 
{
  setPinModes(); 
  MCPsetup();                           
  Serial.begin(9600);
  //mcpWrite(GPIOA, 0b10000000);
  //set Timer 1 interrut to call "decrement" function every second
  Timer1.initialize(1000000);   
  Timer1.attachInterrupt(tick); // set clock to tick every 500ms 
}

 
// -------- Microwave variables ----------
  int typedMins = 0;
  int typedNum;
  char powerSetting;
   
  volatile int cookTimeRemaining = 0;
  volatile int tickCount = 10;


// -------- Microwave states -------------
  volatile boolean startCook  = 0; 
  volatile boolean pause      = 0;
  volatile boolean clearTime  = 0;    
  volatile boolean finishCook = 0;
  volatile boolean doorOpen   = 0; 


// -------- LOOP ------------- 
void loop() 
{
  readKeys();
  
  if(doorOpen == true)
  { 
    turnLamp("ON");
    displayTime(cookTimeRemaining);
  }
  
  else// if(doorOpen == false)
  {
    if (startCook == true) 
    {
      displayTime(cookTimeRemaining);
      cook(powerSetting,cookTimeRemaining);  
    }
    
    else if (pause == true)
    {
      displayTime(cookTimeRemaining);
      turnLamp("OFF");
    }
 
    else if (clearTime == true)
    {
      typedNum = 0;
      displayInt(typedMins);
      clearTime = false;
    }
    
    else if (finishCook == true)
    {
      displayInt(cookTimeRemaining);
      turnLamp("OFF");
    }
    
    else //(!startCook && !pause && !clearTime) take Input
    {
      turnLamp("OFF");
      typedMins = typedNum;
      displayInt(typedMins);

      cookTimeRemaining = numToSec(typedMins); //maybe move to takeInput == 0

      powerSetting = readPowerPot();
      displayChar(powerSetting);
      
    }
  }
//  Serial.print("1:");
//  Serial.print(startCook);
//  Serial.print(" ");
//  Serial.print("2:");
//  Serial.print(pause);
//  Serial.print(" ");
//  Serial.print("3:");
//  Serial.print(clearTime);
//  Serial.print(" ");
//  Serial.print("4:");
//  Serial.print(finishCook);
//  Serial.print(" ");
//  Serial.print("5:");
//  Serial.println(doorOpen);
}


int callbackKeypad(int key)
{
   if ((key >=0)&&(key<=9))
   {
    if(!startCook)
    {
     int typedDig = key;
     typedNum = ((typedNum%1000)*10)+ typedDig;
     tone(BUZZER,NOTE_GS6,100);  
    }
   } 
   
   else if(key == 0xA)
   {
     typedNum+=30;
     cookTimeRemaining+=30;
     tone(BUZZER,NOTE_GS6,100);  
   }

   else if(key == 0xB)
   {
     typedNum+=60;
     cookTimeRemaining+=60;
     tone(BUZZER,NOTE_GS6,100);  
   }
}

void callbackBtn(int btnNo, int state)
{
  switch(btnNo)
  {
    case BTN_START:
      if(cookTimeRemaining != 0 && doorOpen == false)
      {
        tone(BUZZER,NOTE_GS6,100);
        if(pause == true){pause = false;}
        startCook = true;
      }
      break;
    
    case BTN_STOP:
      
      if(doorOpen == false)
      {
        tone(BUZZER,NOTE_GS6,100);
        if     (startCook  == true)  {startCook  = false;  pause     = true;}
        else if(pause      == true)  {pause      = false;  clearTime = true;}
        else if(finishCook == true)  {finishCook = false;  clearTime = true;}
        else                                               clearTime = true;
      }
      break;
    
    case DOOR_SW:
      if(state == 1)
      { 
        doorOpen = true;
        cook(0,cookTimeRemaining);
        if      (finishCook == true){finishCook = false; clearTime = true;}
        else if (startCook  == true){startCook  = false; pause     = true;}
      }
      else if (state == 0)
      {
        doorOpen = false;
        turnLamp("OFF");      
      }
      break;
  }
}


// -------------- Timer interrup function ------------------------
void tick()
{
  
  //------------ Clock logic ------------
   if (cookTimeRemaining > 0 && startCook == true)
   {         
     cookTimeRemaining--;
   }         
   else if (cookTimeRemaining == 0 && startCook == true) 
   {                   
     startCook  = false;
     finishCook = true;
     tickCount = 0; // reset tickCount so the sound of the can be turned on
   }
   
  //------ Buzzer logic -----
  tickCount++;
  if(tickCount <= 5) // buzz 5 times
  {
    tone(BUZZER,NOTE_GS6,500);
  }
}
