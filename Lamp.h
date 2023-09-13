void turnLamp(int lampState)
{
  if (lampState == "ON")
  {
    analogWrite(LAMP,255);
  }
  else if (lampState == "OFF")
  {
    digitalWrite(LAMP,LOW); //20% of 255 
  }
}
