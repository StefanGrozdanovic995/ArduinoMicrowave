void displayTime(int t);
void displayInt(int num);
void displayChar(char powerSetting);
void displayDigit(int place, int number);
int numToSec(int typedMins);

boolean potDisp = false;

const int letterP       = 13;
const int degree        = 12;
const int colon         = 11;
const int decimalPoint  = 10;

const byte digit[]={0X01,  //0b00000001 1st digit
                    0X02,  //0b00000010 2nd digit
                    0X04,  //0b00000100 3rd digit
                    0X08,  //0b00001000 4th digit
                    0X10}; //0b00010000 decimal point place


const byte displaySegments []= {0b00111111, //0
                                0b00000110, //1
                                0b01011011, //2
                                0b01001111, //3
                                0b01100110, //4
                                0b01101101, //5
                                0b01111101, //6
                                0b00000111, //7
                                0b01111111, //8
                                0b01101111, //9
                                0b10000000, //DP
                                0b00000100, //colon
                                0b00001000, //degree
                                0b01110011};//P

void displayInt(int num)
{
  
  int dig;
  if(!potDisp)
  {
    for (int i = 0; i<4; i++)
    {
      dig = num % 10;
      displayDigit(i, dig);
      num = num / 10; 
    }
  }
}
void displayTime(int t)
{
  int seconds;
  int minutes;

  minutes = t / 60;
  seconds = t % 60;

  displayDigit(0,minutes/10); 
  displayDigit(1,minutes%10); 
  displayDigit(2,seconds/10); 
  displayDigit(3,seconds%10);
  displayDigit(4,colon); 
}

void displayChar(char powerSetting)
{
  int powerLevel = powerSetting - 48;
  static int prevPowerLevel;
  static int displayTime = 0;
  if (powerLevel != prevPowerLevel)
  {
    potDisp = true;
    
    if(millis()- displayTime < 1000)
    {
      displayDigit(0, powerLevel);
      delay(1);
      displayDigit(1, letterP);
      delay(1);
    }
    else
    {
       prevPowerLevel = powerLevel;
       potDisp = 0;
       displayTime = millis();
    }    
  } 
}

void displayDigit(int place, int number)
{
  digitalWrite(LATCH,HIGH);
  shiftOut(DATA,CLK,MSBFIRST,displaySegments[number]);
  shiftOut(DATA,CLK,MSBFIRST,digit[place]);
  digitalWrite(LATCH,LOW);      
}

int numToSec(int typedMins)
{
  
}
