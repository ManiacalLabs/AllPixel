Follow the instructions below to install ATUSB_Core

- Install Arduino IDE 1.05r2 or above.
- Download Teensy from [PJRC.com](https://www.pjrc.com/teensy/td_download.html)
- Run Teensy installer as (admin on Windows)
    -Skip driver install (unless you will actually use a Teensy)
    -Install any added libraries if desired
- Copy the ATUSB_Core folder into ```<InstallDir>/Arduino/hardware/``` or ```<UserDocs>/Arduino/hardware/```
- Restart Arduino

You should now have an option for ```ATUSB_32U4_Serial``` in the Boards menu.

For a new, raw ATMega32u4 chip, choose "Burn Bootloader" from the Arduino tools menu to automatically burn the required fuses.

For details on the pin mappings see [core_pins.h](/cores/atusb/core_pins.h)

Once firmware from this core has been uploaded to the device, you should be prompted for driver for AVR USB Serial. Point it to ./drivers/GenericSerialUSB.inf in the ATUSB_Core folder.

On Windows 8, it will not let unsigned drivers be installed without jumping through a few hoops. Follow [this tutorial](http://www.howtogeek.com/167723/how-to-disable-driver-signature-verification-on-64-bit-windows-8.1-so-that-you-can-install-unsigned-drivers/) to walk through the process.
