

#include "Arduino.h"
#include <SAA1064.h>
#include <Wire.h>


#define SAA1064_I2C_ADDRESS 	(0x38)	/* I2C address for 7-Segment */
#define MAX_BRIGHTNESS			(7)
#define NUM_DIGITS				(4)

#define SEG_A					(1 << 0)
#define SEG_B					(1 << 1)
#define SEG_C					(1 << 2)
#define SEG_D					(1 << 3)
#define SEG_E					(1 << 4)
#define SEG_F					(1 << 5)
#define SEG_G					(1 << 6)
#define SEG_DP					(1 << 7)


/***********************************************************************************
 * 
 * Constant array to store the active segments which create the corresponding 
 * decimal or hexadecimal number.
 * 
 **********************************************************************************/
const uint8_t NumberLookup[] = 
{
	SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F,		// 0
	SEG_B|SEG_C,								// 1
	SEG_A|SEG_B|SEG_E|SEG_D|SEG_G,				// 2
	SEG_A|SEG_B|SEG_C|SEG_D|SEG_G,				// 3
	SEG_B|SEG_C|SEG_F|SEG_G,					// 4
	SEG_A|SEG_C|SEG_D|SEG_F|SEG_G,				// 5
	SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,		// 6
	SEG_A|SEG_B|SEG_C,							// 7
	SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,	// 8
	SEG_A|SEG_B|SEG_C|SEG_F|SEG_G,				// 9
	SEG_A|SEG_C|SEG_E|SEG_F|SEG_G,				// A
	SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,				// b
	SEG_A|SEG_D|SEG_E|SEG_F,					// C
	SEG_B|SEG_C|SEG_D|SEG_E|SEG_G,				// d
	SEG_A|SEG_D|SEG_E|SEG_F|SEG_G,				// E
	SEG_A|SEG_E|SEG_F|SEG_G						// F
};	


/***********************************************************************************
 * 
 * Constant array to store the active segments which create the corresponding 
 * character.
 * 
 **********************************************************************************/
const uint8_t CharLookup[] = 
{
	SEG_A|SEG_C|SEG_E|SEG_F|SEG_G,				// 'A'
	SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,				// 'b'
	SEG_A|SEG_D|SEG_E|SEG_F,					// 'C'
	SEG_B|SEG_C|SEG_D|SEG_E|SEG_G,				// 'd'
	SEG_A|SEG_D|SEG_E|SEG_F|SEG_G,				// 'E'
	SEG_A|SEG_E|SEG_F|SEG_G,					// 'F'
	SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G,		// 'g'
	SEG_C|SEG_E|SEG_F|SEG_G,					// 'h'
	SEG_B|SEG_C,								// 'i'
	SEG_B|SEG_C|SEG_D|SEG_E,					// 'J'
	SEG_D|SEG_E|SEG_F,							// 'L'
	SEG_C|SEG_E|SEG_G,							// 'n'
	SEG_C|SEG_D|SEG_E|SEG_G,					// 'o'
	SEG_A|SEG_B|SEG_E|SEG_F|SEG_G,				// 'P'
	SEG_A|SEG_B|SEG_C|SEG_F|SEG_G,				// 'q'
	SEG_E|SEG_G,								// 'r'
	SEG_A|SEG_C|SEG_D|SEG_F|SEG_G,				// 'S'
	SEG_D|SEG_E|SEG_F|SEG_G,					// 't'
	SEG_C|SEG_D|SEG_E,							// 'u'
	SEG_B|SEG_C|SEG_D|SEG_F|SEG_G,				// 'y'	
	SEG_D,										// '_'
	SEG_G,										// '-'
	SEG_DP										// '.'
};



// Constructor
SAA1064::SAA1064()
{

}



void SAA1064::begin()				// Initialize interfaces
{  
	Wire.begin();        					// Join I2C bus
	this->config7SegmentDriver();			// Configure the display driver IC
}


void SAA1064::send7Seg(uint8_t digit, uint8_t segData) // Write a single display digit
{
	Wire.beginTransmission(SAA1064_I2C_ADDRESS);
	Wire.write(digit);
	Wire.write(segData);
	Wire.endTransmission();
}


void SAA1064::showDigit(uint8_t digit, uint8_t number) // Write a single display digit
{
	this->send7Seg(digit, NumberLookup[number]);
}


void SAA1064::showDigit(uint8_t digit, uint8_t number, uint8_t dp) // Write a single display digit
{
	if(dp != 0)
	{
		this->send7Seg(digit, (NumberLookup[number] | SEG_DP));
	}
	
	else
	{
		this->send7Seg(digit, NumberLookup[number]);
	}
}


void SAA1064::showNumber(int32_t number, uint8_t decimalPlace)		// Write a number to the display
{
	static uint8_t digit[4];
	static uint8_t index;
	
	if ((number >= 0) && (number < 10000))
	{
		for (index = 0; index < 4; index++)
		{
			digit[index] = number % 10;
			number = number / 10;
			this->showDigit(index + 1, digit[index], (index == decimalPlace));
		}
	}
	
	else if ((number < 0) && (number > -999))
	{
		number = -number;
		
		this->send7Seg(4, 0x40);
		
		for (index = 0; index < 3; index++)
		{
			digit[index] = number % 10;
			number = number / 10;
			this->showDigit(index + 1, digit[index], (index == decimalPlace));
		}
		
		digit[3] = '-';
	}
}


int32_t SAA1064::float2int32 (float number, uint8_t * decimalPlace)
{
	if (number >= 0)
	{
		if (number < 10)			// if number is up to 9.999
		{
			*decimalPlace = 3;
			number = number * 1000;
		}
		
		else if (number < 100)		// if number is up to 99.99
		{
			*decimalPlace = 2;
			number = number * 100;
		}
		
		else if (number < 1000)		// if number is up to 999.9
		{
			*decimalPlace = 1;
			number = number * 10;
		}
		
		else if (number < 10000)	// if number is up to 9999.
		{
			*decimalPlace = 0;
		}
	}
	
	else
	{
		if (number > -0.01)			// if number is up to -0.009
		{
			*decimalPlace = 3;
			number = 0;
		}
		
		else if (number > -10)		// if number is up to -9.99
		{
			*decimalPlace = 2;
			number = number * 100;
		}
		
		else if (number > -100)		// if number is up to -99.9
		{
			*decimalPlace = 1;
			number = number * 10;
		}
		
		else if (number > -1000)	// if number is up to -999.
		{
			*decimalPlace = 0;
		}
	}
	
	return number;
}


/***********************************************************************************
 * 
 * Function name: AlphaLookup(ascii)
 * 
 * Scope: local
 * 
 * Parameters:
 * 		ascii - ASCII-coded character (note: not all characters are supported)
 * 
 * Return:
 * 		uint8_t - active segments which create the corresponding alphanumeric 
 * 			character from lookup table.
 * 
 * Description:
 * 		AlphaLookup returns the segment data corresponding to the ASCII-coded
 * character passed to the function.  Only certain alphanumeric characters are 
 * supported due to the type of display.  A zero is returned for any unsupported
 * characters, which effectively turns off all 7 segments. 
 * 
 **********************************************************************************/
uint8_t AlphaLookup(char ascii)
{
	switch(ascii)
	{
		case '0': return NumberLookup[0];
		case '1': return NumberLookup[1];
		case '2': return NumberLookup[2];
		case '3': return NumberLookup[3];
		case '4': return NumberLookup[4];
		case '5': return NumberLookup[5];
		case '6': return NumberLookup[6];
		case '7': return NumberLookup[7];
		case '8': return NumberLookup[8];
		case '9': return NumberLookup[9];
		case 'A': return CharLookup[0];
		case 'b': return CharLookup[1];
		case 'C': return CharLookup[2];
		case 'd': return CharLookup[3];
		case 'E': return CharLookup[4];
		case 'F': return CharLookup[5];
		case 'g': return CharLookup[6];
		case 'h': return CharLookup[7];
		case 'i': return CharLookup[8];
		case 'J': return CharLookup[9];
		case 'L': return CharLookup[10];
		case 'n': return CharLookup[11];
		case 'o': return CharLookup[12];
		case 'P': return CharLookup[13];
		case 'q': return CharLookup[14];
		case 'r': return CharLookup[15];
		case 'S': return CharLookup[16];
		case 't': return CharLookup[17];
		case 'u': return CharLookup[18];
		case 'y': return CharLookup[19];
		case '_': return CharLookup[20];
		case '-': return CharLookup[21];
		case '.': return CharLookup[22];
		case ':': return CharLookup[23];
		case 167: return CharLookup[24];
		default: return 0;
	}
}


void SAA1064::showText(char * string)			// Write a pseudo char to a display digit
{
	static uint8_t digit;
	static uint8_t segData;

	if (strlen(string) > NUM_DIGITS)
	{
		digit = NUM_DIGITS;
	}
	
	else
	{
		digit = strlen(string);
	}
	
	while(digit > 0)
	{
		segData = AlphaLookup(string[NUM_DIGITS - digit]);
		this->send7Seg(digit, segData);
		digit--;
	}
}


void SAA1064::clear()					// Clear the display
{
	static uint8_t count;
	
	Wire.beginTransmission(SAA1064_I2C_ADDRESS);
	Wire.write(1);
	
	for(count = 0; count < 4; count++)
	{
		Wire.write(0);
	}
	
	Wire.endTransmission();
}


void SAA1064::off()						// Turn off the display
{
	Wire.beginTransmission(SAA1064_I2C_ADDRESS);
	Wire.write(0);
	Wire.write(0b00000000);
	Wire.endTransmission();
}


void SAA1064::on()						// Turn on the display
{
	Wire.beginTransmission(SAA1064_I2C_ADDRESS);
	Wire.write(0);
	Wire.write(0b01000111);
	Wire.endTransmission();
}


void SAA1064::testOn()						// Run the display test routine
{
	Wire.beginTransmission(SAA1064_I2C_ADDRESS);
	Wire.write(0);
	Wire.write(0b01001111);
	Wire.endTransmission();
}


void SAA1064::testOff()						// Stop the display test routine
{
	Wire.beginTransmission(SAA1064_I2C_ADDRESS);
	Wire.write(0);
	Wire.write(0b01000111);
	Wire.endTransmission();
}


void SAA1064::setBrightness(uint8_t brightness)	// Run the display test routine
{
	if(brightness > MAX_BRIGHTNESS)
	{
		brightness = MAX_BRIGHTNESS;
	}
	
	brightness = 0b00000111 | (brightness << 4);
	
	Wire.beginTransmission(SAA1064_I2C_ADDRESS);
	Wire.write(0);
	Wire.write(brightness);
	Wire.endTransmission();
}


void SAA1064::animate()						// Run the display test routine
{
	static uint8_t counter;
	static uint8_t counter2;

	for (counter = 0; counter < 8; counter++)
	{
		Wire.beginTransmission(SAA1064_I2C_ADDRESS);
		Wire.write(1);
		
		for (counter2 = 0; counter2 < 4; counter2++)
		{
			Wire.write(1 << counter);
		}
		
		Wire.endTransmission();
		delay(100);
	}
}


void SAA1064::config7SegmentDriver()			// Configure the display driver IC
{
	// Configure 7-Segment to 12mA segment output current, Dynamic mode
	//	and Digits 1, 2, 3 AND 4 are NOT blanked    
	Wire.beginTransmission(SAA1064_I2C_ADDRESS);    
	Wire.write(0);
	Wire.write(0b01000111);
	Wire.endTransmission();
}
