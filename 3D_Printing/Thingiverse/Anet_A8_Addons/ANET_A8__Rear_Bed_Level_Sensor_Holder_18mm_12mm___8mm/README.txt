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


http://www.thingiverse.com/thing:2105275
ANET A8 | Rear Bed Level Sensor Holder (18mm, 12mm & 8mm) by TNS is licensed under the Creative Commons - Attribution - Non-Commercial - Share Alike license.
http://creativecommons.org/licenses/by-nc-sa/3.0/

# Summary

**Update April 2017:** If you are looking for more customization, see also [Version 2](http://www.thingiverse.com/thing:2219963)!

<br>
I like the nice design of [LucaCap](http://www.thingiverse.com/LucaCap)'s rear bed leveling sensor holder (http://www.thingiverse.com/thing:1883473). All credit for the fantastic design and idea goes to him! Nevertheless, the original design requires to tap threads into the metal bearing.

This modified design is equipped **with plasic dowels**, which fit in the (non-threaded) holes on the back of the linear slides and are secured with M3 screws (e.g. M3 x 10mm). **NO THREAD TAPING REQUIRED!**

The download section holds a version for a 18mm, a 12mm as well as for a 8mm sensor!

**Note:** depending on your sensor and your cable management you may loose some centimeters of available print height due to a interference of sensor and printer frame.

<br>
**Firmware configuration:**
Due to the altered sensor position the following changes have to be done in the `configuration.h` file (search vor the capitalized string and adapt the subsequent numbers):
*[With my E3Dv6 mount / carriage](http://www.thingiverse.com/thing:2099577):*
`#define X_PROBE_OFFSET_FROM_EXTRUDER 0`
`#define Y_PROBE_OFFSET_FROM_EXTRUDER 50`

*With the stock carriage:*
`#define X_PROBE_OFFSET_FROM_EXTRUDER 26`
`#define Y_PROBE_OFFSET_FROM_EXTRUDER 50`

*Additionally for all setups:*
`#define FRONT_PROBE_BED_POSITION 50`



<br>
(Note for those who don't own a sensor yet: I use [this sensor](http://amzn.to/2kpZqi4))

# Print Settings

Printer: ANET A8
Supports: Yes
Resolution: 0.2
Infill: 50%

Notes: 
**IMPORTANT: Supports definitely required!**

Carefully remove the supports (e.g. with a sharp cutter), without breaking the dowels.

# Post-Printing

![Alt text](https://cdn.thingiverse.com/assets/da/8a/b0/4c/b3/Inst1.png)
Step 1: Push in the sensor brackets. Be carfull not to break the dowels.

![Alt text](https://cdn.thingiverse.com/assets/4f/28/ef/e3/9e/Inst2.png)
Step 2: Lock the brackets with M3 screws (e.g. M3x10).

![Alt text](https://cdn.thingiverse.com/assets/fd/bb/ed/80/3b/Inst3.png)
Step 3: Insert your sensor into the mount. (Either screw it down or carefully widen the brackets with a small plier and push it through)