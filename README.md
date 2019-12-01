# Charlieplexed Christmas Tree

Arduino code for a charlieplexed christmas tree with 2x20 LEDs.
The LEDs are controlled by an ATtiny85.


## Installing Arduino IDE and ATtiny programmer

1) Download and install Arduino IDE

2) Open Preferences in Arduino IDE. Add "Additional Board Manager URL": https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
(See [damellis/attiny on GitHub](https://github.com/damellis/attiny) for more information)

3) Use [Tiny AVR programmer](https://www.sparkfun.com/products/11801) from Sparkfun to program the ATtiny (other programmers may also work). 

4) On Windows you need to install a driver (should not be required on Linux or Mac). The driver can be installed using the [Zadig driver installation tool](https://zadig.akeo.ie). Use Zadig to install "libusb-win32" driver. For more information see Sparfkun guide: https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/

5) Once the driver has been installed. Open Arduino IDE with a blank project. Use the board manager to add the ATtiny boardmanager from damellis

6) In Tools select.

    - Board: ATtiny25/45/85
    - Processor: ATtiny85
    - Clock: Internal 8MHz
    - Programmer: USBtinyISP

7) Insert an ATtiny85 chip into the programmer and connect it to the PC. NOTE: Be careful to insert the chip the right way!

8) In Arduino IDE: Select Tools -> Burn Bootloader


## Install Visual Studio Code and PlatformIO


1) Install VS Code
2) Install PlatformIO extension in VS Code
3) To build: Click checkmark in the statusbar at the bottom of the window (or hit Ctrl+Alt+b)
4) To upload: Click right arrow in the statusbar at the bottom of the window (or hit Ctrl-Alt-u)




