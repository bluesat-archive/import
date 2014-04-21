Flight Computer
===============

The flight computer controls balloon flight specific functions.

The primary functions are balloon cutaway control and parachute cutaway after landing.

The secondary functions are redundancy for tracking and monitoring the satellite if the satellites primary functions fail (or otherwise behave outside expectations).

Communications
--------------

### Opinion
We need 2-way communications with the flight computer. It is useless to have a secondary system if we cannot verify its function. In the case of using the Raspberry Pi, 2-way communications provides full diagnostic capabilities via remote serial console.

At minimum, we need to heartbeat flight telemetry back from form the flight hardware to the ground station in order to maximize the balloons usefulness.


