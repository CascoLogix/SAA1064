

#include <Wire.h>
#include <SAA1064.h>


SAA1064 display;


void setup()
{
  Serial.begin(9600);
  display.begin();
}


void loop()
{
  display.clear();
  display.testOn();
  delay(1000);
  display.testOff();
  delay(1000);
  display.animate();
  delay(1000);
  display.showText("----");
  delay(1000);
  
  countUp();
  delay(1000);
  countDown();
  delay(1000);
}


void countUp (void)
{
  int16_t count;
  
  for (count = 0; count < 10000; count++)
  {
    display.showNumber(count, 0);
    delay(50);
  }
}


void countDown (void)
{
  int16_t count;
  
  for (count = 9999; count >= 0; count--)
  {
    display.showNumber(count, 0);
    delay(50);
  }
}
