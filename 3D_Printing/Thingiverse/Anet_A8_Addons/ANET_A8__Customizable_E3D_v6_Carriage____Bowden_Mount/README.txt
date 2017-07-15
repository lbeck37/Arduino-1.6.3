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


http://www.thingiverse.com/thing:2099577
ANET A8 | Customizable E3D v6 Carriage /  Bowden Mount by TNS is licensed under the Creative Commons - Attribution - Non-Commercial - Share Alike license.
http://creativecommons.org/licenses/by-nc-sa/3.0/

# Summary

Hi Thingiverse-Community!

<br>
** *UPDATE March 27th: Added option for parameterizable sensor holder in CUSTOMIZER.* **
<br>

I was not satisfied with the stock hotend / extruder setup of the ANET A8! I had a couple of issues with the old one and I didn't like the following things:
* Accessibility of the extruder motor was bad (unmount fan with cooling plate)
* It was hard to actually watch the nozzle during print due to a clustered design with lots of components attached to the 'print-head'
* High print speeds went along with more inaccurate results, due to the heavy 'print-head' with the extruder motor

The design is inspired / based on the design by [Jubaleth](http://www.thingiverse.com/Jubaleth) (http://www.thingiverse.com/thing:2002778).

<br>
**Features of this mount:**
* Slim, light and modular design
* Unobstructed view on nozzle / as few things as possible in front of nozzle (cooling fan moved to left side, leveling sensor on back)
* Equipped with several slots for zip ties for a proper cable management
* Cooling fan in (nearly) same distance to the nozzle as before -> you have good chances to still use your current cooling duct
* Fully compatible with x-axis cable chain (http://www.thingiverse.com/thing:2105040)
* Customizable sensor holder (optional in customizer interface) - ***Attention:** this sensor mount may interfere with other components (e.g. a cable chain), I still recommend the rear sensor holder (see links below)*

<br>
**Pick your version / file:**
* **CarriageV2_3__63mm_AnetA8_E3Dv6_Mount.stl** - For hotends with a total height of ~63mm (top to nozzle tip - measure *without* push-fit), eg. *original E3Dv6*
* **CarriageV2_3__74mm_AnetA8_E3Dv6_Mount.stl** - For hotends with a total height of ~74mm (top to nozzle tip - measure *without* push-fit)
* **Use the CUSTOMIZER!** - Just set *your* personal preferences and export *your* custom carriage! (Caution: settings in '*expert*' tab are not foolproof and may need firmware adaptations (other then them in this description)!)

**DISCLAIMER:** There are several variations of hotends out there, I tried to adapt this carriage to the most of them! Nevertheless, in some cases the fan position might be slightly off. You can use the 'expert-fine-tuning' in the CUSTOMIZER to fine tune the position of the fan holder to suite you personal needs or to fit your beloved fan duct! (I used the same coordinate system orientation as the printer in order to make those adjustment parameters as easy graspable as possible.)

<br>
**Modular extensions and recommended addons:**
* [Bowden extruder motor mount](http://www.thingiverse.com/thing:2078038) (Anet A8 stock parts) or [improved bowden extruder](http://www.thingiverse.com/thing:2204941) (non-stock parts)
* [Rear bed leveling sensor mount](http://www.thingiverse.com/thing:2105275) or [rear sensor mount V2](http://www.thingiverse.com/thing:2219963) (screw- and customizable)
* [Belt holder](http://www.thingiverse.com/thing:1433295) (by Simhopp)
* [Hotend cooling mount for the E3D v6 clones](http://www.thingiverse.com/thing:962161) (by anthonywob)
* [Mistral 2 fan duct](http://www.thingiverse.com/thing:2121279) (by Leo) - rotate your heater block in a way that the shorter end points towards the fan

<br>
**Firmware Adaptation:**
Due to a changed nozzle position it is recommended to change the following parameter in your `configuration.h` file:
`#define X_MIN_POS     -10`
*NOTE:* If this parameter is not changed, all prints will be shifted around 1 cm to the right.

If you added a sensor holder, you also have to adapt the probe position in the firmware correspondingly!

<br>
**Why should I switch to a bowden setup?**
A bowden setup has the benefit of moving the (heavy) extruder motor away from the moving carriage. Therefore the printer is able to accelerate way faster due to the reduced weight of the carriage. Based on this fact, the printer is able to achieve higher speeds without getting inaccurate results.

<br>
**Why do you exchange the stock hotend with a E3D v6 styled one?**
In order to avoid jams or clogges it is crucial to have a hot nozzle, but still a cold shaft! Compared to the stock version, E3D v6 styled nozzles provide a large cooling surface which will keep the shaft super cool without any effort!

<br>
**Things you will need in order to switch to this setup:**
* [E3Dv6](http://e3d-online.com/E3D-v6/Full-Kit/v6-1.75mm-Universal-Bowden) / [E3D clone](http://amzn.to/2ljysIe)
* [Teflon tube and pneumatic couplers](http://amzn.to/2lP6dPo)


# Print Settings

Printer: ANET A8
Rafts: No
Supports: No
Resolution: 0.2
Infill: 40

# Post-Printing

## Quick Installation Guide

This is just a quick installation guide with some images for the basic steps.

![Alt text](https://cdn.thingiverse.com/assets/a1/f1/88/a9/a6/Inst1.jpg)
Step 1: After unmoting stock metal frame, screw hotend mount with stock screws to the sliders.

![Alt text](https://cdn.thingiverse.com/assets/37/78/15/dd/05/Inst2.jpg)
Step 2: Attach the belt tensioner / belt holder to the back of the mount with 6 screws. (OPTIONAL: Attach auto leveling sensor to the back as seen in the image) [Links to those parts can be found in the description]

![Alt text](https://cdn.thingiverse.com/assets/4f/c7/2e/bf/13/Inst3.jpg)
Step 3: Insert the hotend and secure it with the coresponding cap.

![Alt text](https://cdn.thingiverse.com/assets/32/74/98/e5/dc/Inst4.jpg)
Step 4: Attach the stock cooling fan with the stock screws to the side frame.

![Alt text](https://cdn.thingiverse.com/assets/81/05/5f/f8/8c/Inst5.jpg)
Step 5: Attach cooling fan for hotend. Either use the one shipped with your hotend or print an adapter for the stock 40mm fan. [Corresponding links in the description]

![Alt text](https://cdn.thingiverse.com/assets/d9/0d/f7/84/34/Inst6.jpg)
Step 6: Arrange your cables and fix them with zip ties

# Changelog

**Changelog:**
* **02/12/17** - V2.0 - Initial (public) release
* **02/13/17** - V2.1 - Smoother finish and slightly repositioned fan mount
* **02/17/17** - V2.2 - Minor optical fixes
* **02/19/17** - V2.3 - Fixes and optimizations in mesh export
* **02/22/17** - V2.3 - Added Customizer support / version
* **03/05/17** - V2.3 - Updated Customizer version (wider range fan z pos.)
* **03/27/17** - V3.0 - Added option for sensor mount to customizer version