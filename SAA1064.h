

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