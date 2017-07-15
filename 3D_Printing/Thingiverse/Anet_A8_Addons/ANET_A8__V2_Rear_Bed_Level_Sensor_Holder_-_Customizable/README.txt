                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


http://www.thingiverse.com/thing:2219963
ANET A8 | V2 Rear Bed Level Sensor Holder - Customizable by TNS is licensed under the Creative Commons - Attribution - Non-Commercial - Share Alike license.
http://creativecommons.org/licenses/by-nc-sa/3.0/

# Summary

I further remixed the rear sensor holder... This time a **customizable** design and featuring also a *screwable clamp* for easy installation and removement! The general idea is from [LucaCap](http://www.thingiverse.com/LucaCap)'s rear bed leveling sensor holder (http://www.thingiverse.com/thing:1883473). All credit for the fantastic design and idea goes to him! Nevertheless, the original design requires to tap threads into the metal bearing.

This modified design is equipped **with plasic dowels**, which fit in the (non-threaded) holes on the back of the linear slides and are secured with M3 screws (e.g. M3 x 10mm). **NO THREAD TAPING REQUIRED!**

The download section holds the basic vairants for a 18mm, a 12mm as well as for a 8mm sensor! **For further options or versions use the CUSTOMIZER!**

**Variants:**
* **Clip** - as the version before with further improvements and customizable parameters
* **Screw** - use a screwable clamp to secure your sensor to the back of your linear slide! This enables easy installation and removement and does not allow the sensor to move at all.

**Note:** depending on your sensor and your cable management you may loose some centimeters of available print height due to a interference of sensor and printer frame.

<br>
**Firmware configuration:**
Due to the altered sensor position the following changes have to be done in the `configuration.h` file (search vor the capitalized string and adapt the subsequent numbers):
*[With my E3Dv6 mount / carriage](http://www.thingiverse.com/thing:2099577):*
`#define X_PROBE_OFFSET_FROM_EXTRUDER 0`
`#define Y_PROBE_OFFSET_FROM_EXTRUDER 51`

*With the stock carriage:*
`#define X_PROBE_OFFSET_FROM_EXTRUDER 26`
`#define Y_PROBE_OFFSET_FROM_EXTRUDER 51`

*Additionally for all setups:*
`#define FRONT_PROBE_BED_POSITION 51`

** *NOTE:* ** If you added a custom offset, adapt the Y-parameters above correspondingly!


<br>
(Note for those who don't own a sensor yet: I use [this sensor](http://amzn.to/2kpZqi4))

# Print Settings

Rafts: No
Supports: Yes
Resolution: 0.2
Infill: 40%

Notes: 
**Recommended Screws:**
 * Mount/Dowels: M3x10
 * Clamp: M3x10