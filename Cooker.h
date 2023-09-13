void cook(char powerSetting, int cookTimeRemaining)
{
  static int prevCookTime = 0;
  static int counter = 0;
  int maxPowerLevel = 5;
  
  int powerLevel = powerSetting - 48;
  
  if(cookTimeRemaining != cookTimeRemaining)
  {
    counter ++;
  }

  if(counter / powerLevel == 0)
  {
    analogWrite(LAMP,128); //50% of 255
  }
  else if(counter / powerLevel <= maxPowerLevel)
  {
    analogWrite(LAMP,51); //20% of 255
  }
  else
  {
    counter = 0;
  }
  
  prevCookTime = cookTimeRemaining;
}
