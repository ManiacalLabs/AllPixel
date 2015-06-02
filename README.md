AllPixel - SmartMatrix Variant
========

This is a variant of the AllPixel firmware specifically for controlling SmartMatrix compatible LED displays using the [SmartMatrix Teensy Shield](http://docs.pixelmatix.com/SmartMatrix/shieldref.html) and the [BiblioPixel](https://github.com/ManiacalLabs/BiblioPixel) Python library.

This type of display is **not** like other LED strips that the AllPixel normally controls. It is a "true" matrix display where the pixels are multiplexed. They can be purchased through sites such as [HackADay](http://store.hackaday.com/products/smartmatrix-bundle). The linked bundle even includes the required SmartMatrix Teensy Shield :)

Setup
=====

To use this, you must first build and load the firmware onto a [Teensy 3.1](http://store.hackaday.com/products/teensy-3-1).

To do so, the following things are required:

- Arduino 1.6.4 or greater: http://www.arduino.cc/en/Main/Software
- Teensyduino add-on, v1.23 or greater: https://www.pjrc.com/teensy/td_download.html
- [FastLED v3.1](https://github.com/FastLED/FastLED/tree/FastLED3.1) (dev branch): https://github.com/FastLED/FastLED/archive/FastLED3.1.zip
- [SmartMatrix](https://github.com/pixelmatix/SmartMatrix) Library: https://github.com/pixelmatix/SmartMatrix/releases/download/2.2.0/SmartMatrix_32x32-2.2.0.zip

With these installed, load Firmware.ino and select "Teensy 3.1" from the Boards menu in the Arduino IDE.
Compile and upload the sketch to the Teensy board.

Usage
=====

Now that the firmware is uploaded to the Teensy (we will assume you have build and connected the SmartMatrix shield) it is time to get BiblioPixel working.

Fortunately, it's pretty basic since there really are no options to choose from but you *do* have to set the correct ones.

```python
from bibliopixel.drivers.serial_driver import *
driver = DriverSerial(type = LEDTYPE.GENERIC, num = 32*32, hardwareID = "16C0:0483", dev="")

import bibliopixel.log as log
log.setLogLevel(log.DEBUG)

import bibliopixel.colors as colors

#load the LEDStrip class
from bibliopixel.led import *
led = LEDMatrix(driver, serpentine = False, threadedUpdate=True)

#load channel test animation
from bibliopixel.animation import MatrixCalibrationTest
anim = MatrixCalibrationTest(led)

try:
	anim.run()
finally:
	led.all_off()
	led.update()

time.sleep(0.1)
```

For the DriverSerial instance:

- "type" doesn't matter but just leave it GENERIC. 
- "num" **must** be 32*32 or 1024 
- "hardwareID" is set above to the USB VID/PID of the Teensy 3.1 which is different from the default value used for the AllPixel. For autodetection, this value must be overriden with "16C0:0483". If this does not work, use "dev".
- "dev" is optional and get be set to the serial port name (like "COM5" or "/dev/ttyUSB1") if not using "hardwareID"

For the LEDMatrix instance:

- "serpentine" **must** be False since the display is not a serpentine layout but True is the default value.
- "threadedUpdate" is optional and can sometimes speed up pushing to the display

That's it! For more details on using BiblioPixel check out the [BiblioPixel Wiki](https://github.com/ManiacalLabs/BiblioPixel/wiki).

Please submit any questions or support requests to the [Maniacal Labs Forum](http://forum.maniacallabs.com/).
