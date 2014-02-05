/** 
 *  SAA1064_DemoCount.ino
 *  
 *  This is a demo program for illustrating the functionality of the 
 *  SAA1064.cpp library.
 *
 *  Copyright (C) 2014  Clint Stevenson (CascoLogix http://cascologix.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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
