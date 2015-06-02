#include <SmartMatrix.h>
#include <FastLED.h>
#include <EEPROM.h>
#include "global.h"

#define kMatrixWidth  32
#define kMatrixHeight 32
const bool    kMatrixSerpentineLayout = false;

CRGB * _fastLEDs;


uint16_t numLEDs = (kMatrixWidth * kMatrixHeight);
bool errorPixelCount = false;
uint8_t bytesPerPixel = 3;

inline void setupFastLED()
{
	_fastLEDs = (CRGB*)malloc(sizeof(CRGB)*numLEDs);
	memset(_fastLEDs, 0, sizeof(CRGB)*numLEDs);

	FastLED.addLeds<SMART_MATRIX>(_fastLEDs, numLEDs);

    //FastLED.setDither(DISABLE_DITHER);
	FastLED.clear();
	FastLED.show();
}

void setup()
{
	Serial.begin(1000000);
	Serial.setTimeout(1000);

	if (EEPROM.read(0) != CONFIGCHECK)
	{
		EEPROM.write(0, CONFIGCHECK);
		writeDefaultConfig();
		EEPROM.write(16, 0);
	}

	readConfig();

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
			//doReboot();
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
					temp.pixelCount = temp.pixelCount / bytesPerPixel;

					if(temp.pixelCount != numLEDs){
						result = RETURN_CODES::ERROR_PIXEL_COUNT;
					}

					if (result == RETURN_CODES::SUCCESS && memcmp(&temp, &config, sizeof(config_t)) != 0)
					{
						memcpy(&config, &temp, sizeof(config_t));

						writeConfig(); //save changes for next reboot

						Serial.write(RETURN_CODES::SUCCESS);
						Serial.flush();
						// delay(100);
						// doReboot(); //Reboot and start over
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
