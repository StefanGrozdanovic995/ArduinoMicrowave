char* powerMap[6] = {'0', '1', '2', '3', '4', '5'};

int readPowerPot()
{
  int potVal = analogRead(POT_PIN);
  float powerReading = ((5.0-1.0)/1023.0)*potVal + 1.0;
  int powerLevel = round(powerReading);
  char powerSetting = powerMap[powerLevel];
  return powerSetting;
}
