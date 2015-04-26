#include <FastLED.h>
#include <EEPROM.h>
#include "global.h"
//#include "SPI.h"

CRGB * _fastLEDs;


uint16_t numLEDs = 1;
CLEDController * pLed = NULL;
bool errorPixelCount = false;
uint8_t bytesPerPixel = 3;

inline void setupFastLED()
{
	switch (config.type)
	{
#ifdef GENERIC
	case GENERIC:
		//update this to whatever your setup uses
		bytesPerPixel = 3;
		break;
#endif

		//Most things use 3 bytes per pixel
		//Easier to use a catch all and then handle other cases
	default:
		bytesPerPixel = 3;
		break;
	}

	if (((freeRam() - FREE_RAM_BUFFER) / bytesPerPixel) < config.pixelCount)
	{
		errorPixelCount = true;
		writeDefaultConfig();
		return;
	}

	_fastLEDs = (CRGB*)malloc(sizeof(CRGB)*numLEDs);
	memset(_fastLEDs, 0, sizeof(CRGB)*numLEDs);

	switch (config.type)
	{
#ifdef GENERIC
	case GENERIC:
		//Do setup here for bypassing normal chipset control
		break;
#endif

//SPI Based Chipsets
#ifdef LPD8806
	case LPD8806:
		pLed = new LPD8806Controller < SPI_DATA, SPI_CLOCK, RGB>();
		break;
#endif
#ifdef WS2801
	case WS2801:
		pLed = new WS2801Controller<SPI_DATA, SPI_CLOCK, RGB>();
		break;
#endif
#ifdef SM16716
	case SM16716:
		pLed = new SM16716Controller<SPI_DATA, SPI_CLOCK, RGB>();
		break;
#endif
#ifdef APA102
	case APA102:
		pLed = new APA102Controller<SPI_DATA, SPI_CLOCK, RGB>();
		break;
#endif
#ifdef P9813
	case P9813:
		pLed = new P9813Controller<SPI_DATA, SPI_CLOCK, RGB>();
		break;
#endif

//One Wire Chipsets
#ifdef NEOPIXEL
	case NEOPIXEL:
		pLed = new WS2811Controller800Khz<ONEWIREPIN, RGB>();
		break;
#endif
#ifdef WS2811_400
	case WS2811_400:
		pLed = new WS2811Controller400Khz<ONEWIREPIN, RGB>();
		break;
#endif
#ifdef TM1809_TM1804
	case TM1809_TM1804:
		pLed = new TM1809Controller800Khz<ONEWIREPIN, RGB>();
		break;
#endif
#ifdef TM1803
	case TM1803:
		pLed = new TM1803Controller400Khz<ONEWIREPIN, RGB>();
		break;
#endif
#ifdef UCS1903
	case UCS1903:
		pLed = new UCS1903Controller400Khz<ONEWIREPIN, RGB>();
		break;
#endif
#ifdef LPD1886
	case LPD1886:
		pLed = new LPD1886Controller1250Khz<ONEWIREPIN, RGB>();
		break;
#endif

	default:
		//Nothing happens. No chipset configured. Not exactly an error condition
		break;
	}

	if (pLed)
	{
		FastLED.addLeds(pLed, _fastLEDs, numLEDs);
	}

	setSPIRate(config.spiSpeed);

    //FastLED.setDither(DISABLE_DITHER);
	FastLED.clear();
	FastLED.show();
}

void setSPIRate(uint8_t speed) {
	SPCR &= ~((1 << SPR1) | (1 << SPR0)); 	// clear out the prescalar bits

	bool b2x = false;
	uint8_t _SPI_CLOCK_DIVIDER = DATA_RATE_MHZ(speed);

	if (_SPI_CLOCK_DIVIDER >= 128) { SPCR |= (1 << SPR1); SPCR |= (1 << SPR0); }
	else if (_SPI_CLOCK_DIVIDER >= 64) { SPCR |= (1 << SPR1); }
	else if (_SPI_CLOCK_DIVIDER >= 32) { SPCR |= (1 << SPR1); b2x = true; }
	else if (_SPI_CLOCK_DIVIDER >= 16) { SPCR |= (1 << SPR0); }
	else if (_SPI_CLOCK_DIVIDER >= 8) { SPCR |= (1 << SPR0); b2x = true; }
	else if (_SPI_CLOCK_DIVIDER >= 4) { /* do nothing - default rate */ }
	else { b2x = true; }

	if (b2x) { SPSR |= (1 << SPI2X); }
	else { SPSR &= ~(1 << SPI2X); }
}

void setup()
{
	pinMode(0, OUTPUT);
	digitalWrite(0, HIGH);
	digitalWrite(rebootPin, LOW);
	pinMode(rebootPin, OUTPUT);

	Serial.begin(1000000);
	Serial.setTimeout(1000);

	if (EEPROM.read(0) != CONFIGCHECK)
	{
		EEPROM.write(0, CONFIGCHECK);
		writeDefaultConfig();
		EEPROM.write(16, 0);
	}

	readConfig();
	numLEDs = config.pixelCount;

	setupFastLED();
}

#define EMPTYMAX 100
inline void getData()
{
	static char cmd = 0;
	static uint16_t size = 0;
	static uint16_t count = 0;
	static uint8_t emptyCount = 0;
	static size_t c = 0;
	static uint16_t packSize = numLEDs * bytesPerPixel;

	if (Serial.available())
	{
		cmd = Serial.read();
		size = 0;
		Serial.readBytes((char*)&size, 2);

		if (errorPixelCount)
		{
			Serial.write(RETURN_CODES::ERROR_PIXEL_COUNT);
			Serial.flush();
			delay(100);
			doReboot();
		}
		else if (cmd == CMDTYPE::PIXEL_DATA)
		{
			count = 0;
			emptyCount = 0;

			if (size == packSize)
			{
				while (count < packSize - 1)
				{
					c = Serial.readBytes(((char*)_fastLEDs) + count, packSize - count);
					if (c == 0) 
					{
						emptyCount++;
						if(emptyCount > EMPTYMAX) break;
					}
					else
					{
						emptyCount = 0;
					}

					count += c;
				}
			}

			uint8_t resp = RETURN_CODES::SUCCESS;
			if (count == packSize)
			{
				FastLED.show();
			}
			else
				resp = RETURN_CODES::ERROR_SIZE;

			Serial.write(resp);
		}
		else if(cmd == CMDTYPE::GETID)
		{
			Serial.write(EEPROM.read(16));
		}
		else if(cmd == CMDTYPE::SETID)
		{
			if(size != 1)
			{
				Serial.write(RETURN_CODES::ERROR_SIZE);
			}
			else
			{
				uint8_t id = Serial.read();
				EEPROM.write(16, id);
				Serial.write(RETURN_CODES::SUCCESS);
			}
		}
		else if (cmd == CMDTYPE::SETUP_DATA)
		{
			uint8_t result = RETURN_CODES::SUCCESS;
			config_t temp;

			if (size != sizeof(config_t))
			{
				result = RETURN_CODES::ERROR_SIZE;
			}
			else
			{
				size_t read = Serial.readBytes((char*)&temp, sizeof(config_t));
				if (read != size)
				{
					result = RETURN_CODES::ERROR_SIZE;
				}
				else
				{
					//validate strip type and set pixelCount
					switch (temp.type)
					{
				#ifdef GENERIC
					case GENERIC:
						//update this to whatever your setup uses
						temp.pixelCount = temp.pixelCount / 3;
						break;
				#endif

						//One of these NEEDS to be defined, otherwise comment out the whole block
				#ifdef LPD8806
					case LPD8806:
				#endif
				#ifdef WS2801
					case WS2801:
				#endif
				#ifdef NEOPIXEL
					case NEOPIXEL:
				#endif
				#ifdef WS2811_400
					case WS2811_400:
				#endif
				#ifdef TM1809_TM1804
					case TM1809_TM1804:
				#endif
				#ifdef TM1803
					case TM1803:
				#endif
				#ifdef UCS1903
					case UCS1903:
				#endif
				#ifdef SM16716
					case SM16716:
				#endif
				#ifdef APA102
					case APA102:
				#endif
				#ifdef LPD1886
					case LPD1886:
				#endif
				#ifdef P9813
					case P9813:
				#endif
					case 255: //This is just so that this case remains valid but uncalled if all options are not defined
						temp.pixelCount = temp.pixelCount / 3;
						break;
					default:
						result = RETURN_CODES::ERROR_UNSUPPORTED;
						break;
					}

					if (temp.spiSpeed < 1 || temp.spiSpeed > 24)
						result = RETURN_CODES::ERROR_UNSUPPORTED;
				#ifdef WS2801
					if (temp.type == WS2801)
						temp.spiSpeed = 1;
				#endif

					if (result == RETURN_CODES::SUCCESS && memcmp(&temp, &config, sizeof(config_t)) != 0)
					{
						memcpy(&config, &temp, sizeof(config_t));

						writeConfig(); //save changes for next reboot

						Serial.write(RETURN_CODES::REBOOT); //send reboot needed
						Serial.flush();
						delay(100);
						doReboot(); //Reboot and start over
					}

					//On config we reset the brightness.
					//Otherwise previous brightness values could 
					//still be in memory.
					FastLED.setBrightness(255);
					FastLED.setDither(1);
				}
			}

			Serial.write(result);
		}
		else if (cmd == CMDTYPE::BRIGHTNESS)
		{
			uint8_t result = RETURN_CODES::SUCCESS;
			if (size != 1)
				result = RETURN_CODES::ERROR_SIZE;
			else
			{
				uint8_t brightness = 255;
				size_t read = Serial.readBytes((char*)&brightness, 1);
				if (read != size)
					result = RETURN_CODES::ERROR_SIZE;
				else
				{
					FastLED.setBrightness(brightness);
				}
			}

			Serial.write(result);
		}
		else if (cmd == CMDTYPE::GETVER)
		{
			Serial.write(RETURN_CODES::SUCCESS);
			Serial.write(FIRMWARE_VER);
		}
		else
		{
			Serial.write(RETURN_CODES::ERROR_BAD_CMD);
		}


		Serial.flush();
	}

	//Serial.println(freeRam(), DEC);
}

void loop()
{
	getData();
	FastLED.delay(0);
}
