#include <avr/io.h>
#include "uart.h"
#include <avr/pgmspace.h>
//#define ENABLE_EEPROM_DEMO

char strRom[] PROGMEM = "No se de interrupciones, por lo tanto voy a estudiar";
char strGar[] PROGMEM = "No voy a morir en este semestre";
char strVal[] PROGMEM = "Voy a seguir estudiando, no quiero repetir";
char str2[] PROGMEM = "ya casi son vacaciones";

char* strTable[] = {strRom, strGar, strVal, str2};

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

int main(void)
{
	uint8_t strIdx = 0;
	UART_getchar();

	while(1)
	{

#ifdef ENABLE_EEPROM_DEMO

		for (uint16_t eeIdx = 0; eeIdx < 4096; eeIdx++)
		{
			UART_putchar(EEPROM_read(eeIdx));
		}
#elif defined ENABLE_PGM_DEMO
		for (uint16_t charIdx = 0;pgm_read_byte(strTable[strIdx] + charIdx) != 0 ; charIdx++)
		{
			UART_putchar(pgm_read_byte(strTable[strIdx] + charIdx));
		}
#endif
		UART_puts("\n\r");
		UART_getchar();
		strIdx = (strIdx + 1) & 3;
	}
}