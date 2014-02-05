/** 
 *  SAA1064.h
 *  
 *  This is a library for interfacing to the NXP SAA1064 7-segment display
 *  driver IC.  
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


#ifndef SAA1064_h
#define SAA1064_h


#include <stdint.h>


class SAA1064 {
public:
	SAA1064();								// Constructor
		
	// Display driver access methods
	void begin();							// Initialize interfaces
	void showDigit(uint8_t digit, uint8_t number); // Write a single display digit
	void showDigit(uint8_t digit, uint8_t number, uint8_t dp); // Write a single display digit
	void showNumber(int32_t number, uint8_t decimalPlace);		// Write a number to the display
	void showText(char * string);			// Write a pseudo char to a display digit
	void clear();							// Clear the display
	void off();								// Turn off the display
	void on();								// Turn on the display
	void testOn();							// Run the display test routine
	void testOff();							// Run the display test routine
	void animate();
	void setBrightness(uint8_t brightness);	// Run the display test routine

	// Utility methods
	int32_t float2int32 (float number, uint8_t * decimalPlace);
	
private:
	void send7Seg(uint8_t digit, uint8_t segData); // Write raw data to a single display digit
	void configTemperature();				// Configure the temperature sensor IC
	void config7SegmentDriver();			// Configure the display driver IC
};
#endif // SAA1064_h