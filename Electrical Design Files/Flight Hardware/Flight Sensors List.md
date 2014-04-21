Flight Hardware Sensors
=======================

This is a list of the flight-hardware sensors that need to be somehow controlled on the balloon flight but are _not_ part of the satellite.

_Downlinked_ sensors are things which should be transmitted during flight to the ground station.

_Logging_ Sensors are things which don't need to be transmitted.

How To Use
----------
This is a WIP document. Edits welcome. It is written in Markdown. Just copy the style in a text editor.

To Do
-----
- [ ] Which sensors do we _need_?
- [ ] Which sensors do we _want_?
- [ ] What type or grade of parts are needed. Accuracy and precision numbers.

Downlinked sensors
------------------
* Tertiary GPS
    * Numero(?) board over RS-232 interface.
    * [SPOT GPS Transmitter](http://www.findmespot.com/en/index.php?cid=102#)  
    Requires a service plan, does not use the cell network (some type of satellite solution - GEOS? - very competitively priced).

* Accelerometer
    * Off-the-shelf: [ADXL345 - TRIPLE-AXIS ACCELEROMETER (+-2G/4G/8G/16G) W/ I2C/SPI](https://www.adafruit.com/products/1231)

* Pressure  
Temperature
    * [Altimeter sensor chips](http://www.servoflo.com/sensors-by-application/barometric-pressure-measurement-a-compensation.html)  
    Cover pressure and temperature issues, and give us non-GPS altimetry information. Any of these are ~$20. SPI interface.

* Wind-Speed
    * Do we need/want it?  
    We have position data and velocities from GPS of the actual balloon, and practically, any sensor system is external and will be in extreme conditions while containing moving mechanical components. These devices are not cheap either.

Logging sensors
---------------
* Camera
    * Question: What are we trying to record here?  
        * Promotional pictures of the curvature of the Earth are pretty but not very technically interesting.  
        * External footage of the satellite body would be diagnostic in case of any failures. Thorough coverage would be an external frame with 3 cameras spaced 120 degrees apart to provide a multi-angle view.
    * [GoPro Hero3](http://gopro.com/cameras/hd-hero3-white-edition#technical-specs) is an off-the-shelf, weather-proof option. [Previous models have flown to 24km.](http://www.mit.edu/~adalca/SpaceCam/)
    * Downlinking in-flight footage is going to be energy-expensive and difficult with the available hardware we have, whereas an SD or CF card would probably survive a free-fall from space intact.