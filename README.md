# open-trip

Open Trip is an Open Source **Arduino** project for an all-in-one rally roadbook.

For a complete and useful roadbook setup, you usually need a motor controlled box for the paper roll and two displays so you can see the partial trip in one, and the total trip in the other, or the partial trip in one and the heading degrees in the other. Also, you need 3 buttons to control the displays, and a "joystick" to control the paper roll.

Since the off-the-shelf solutions like the [ICO Rallye](http://www.icoracing.com/collections/ico-racing-products/products/rallye-max-g) for the displays (you need two) and the [MD Roadbook Reader](http://www.rallymanagementservices.com/products/md-roadbook-reader) are quite expensive, I'm making my own setup and sharing everything I learn in the process with you.

My goal is to emulate the most important features of the ICO Rallye so you won't miss anything.

# IMPORTANT

This is currently a work in progress, so the code is functional but not all of the features have been implemented yet. The implementation is going to be incremental and ordered by what I consider prioritary.

I will try to keep this README file as updated as I can, and I'll put a list of the hardware components once I have the definitive ones.

### Hardware
* Two configurable backlit LCD displays
* Magnetometer to measure heading
* Hall effect sensor to measure distance and speed
* Motor to rotate the paper roll
* LED strip to retro-illuminate the paper roll
* Buttons to increase / decrease / reset distance
* Temporary bidirectional switch to control the roadbook motor
* Arduino Nano as the processing unit

### Features
* Menu to configure settings
* Configurable screens to display
	* Partial distance (in kilometers)
	* Total distance (in kilometers)
	* Compass (in degrees)
	* Speed (in kilometers / hour)
* Configurable wheel circumference (in milimeters)
* Configurable backlight (enabled / disabled)
* Automatic calibration option
* Configurable Magnetic Declination (http://www.magnetic-declination.com/)
* Configurable distance measures (Kilometers or Miles)
* All configuration parameters are persisted in EEPROM memory

### Setup

* Install Arduino libraries [to-do]

# Notes

### Wheel circumference and lying odometer

It's a well known fact that cars and motorcycle's speedometers lie. They always read higher than the real value (http://gpssystems.net/accurate-gpsbased-speedometer/).
The reasons for that are beyond this project, but I'm using my GPS to measure distances and try to imitate the results in the *open-trip* odometer.

I measured the wheel in my bike and I got 2007 milimeters. It turns out that I'm getting wrong reads, so after some calculation, I ended up using 2045 milimeters and this gives me the exact same distance number as the GPS, so I'll stick to that value. The bike's own odometer always shows more.

