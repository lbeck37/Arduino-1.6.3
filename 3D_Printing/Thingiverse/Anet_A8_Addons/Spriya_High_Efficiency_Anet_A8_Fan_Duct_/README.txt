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


http://www.thingiverse.com/thing:2133328
"Spriya" - High Efficiency Anet A8 Fan Duct  by lokster is licensed under the Creative Commons - Attribution - Non-Commercial license.
http://creativecommons.org/licenses/by-nc/3.0/

# Summary

## The story
I designed this fan duct as a 100% free & open source alternative to another one which was very good, but unfortunately it's license was preventing me from modifying it the way I see fit & share it freely.
So I started Blender, and made my own from scratch in a couple of days. One that I can share freely with the community, so anyone can use it, remix it or trash it - in any order ;)
The fan duct works very good for me, but I give you no warranty that it will work for you.

## Highlights of this fan duct:
- The most important: it's more effective compared to the semi-circular design, and A LOT more effective than the stock Anet A8 fan duct.
- it's quieter, compared to the stock & the semi circular design
- it does not block the view, and you can see your nozzle tip
- it uses my own attachment design, and once it's inserted onto the fan, it's impossible to fall off. Forget about the tape or glue for securing your "always falling" fan duct.
- it blows more air, compared to the other duct designs I tested. Instead of concentrating "on the tip of the nozzle", the air flow is wider, and separated at 2 flows at approximately 90 degrees. This way it cools wider area, even if the nozzle moves away from the "point of interest".
- when mounted & tuned properly using PID Auto Tune (described below), it does not cool the heat block, and the temperature is very stable - plus/minus 0.5C.
- VERY lightweight - just 3 grams, compared most of the other fan ducts which weigh 4-6 grams or more! So even if you find out it's trash, you've lost the material for just 1/6 3D Benchie or one calibration cube :)
- open source design, made with open source software - I am releasing the original design in .blend format, which you can load in Blender and make modifications. You are then completely free to release any remixed designs you made! In fact, it will be great!
- the mesh is relatively low-poly, so it's easy to edit in blender if you want to make your own adjustments.

## Testing
I used the Marvin Key Chain as a test object - http://www.thingiverse.com/thing:215703.
The print settings were as follows:
- layer height 0.2mm
- shell width 1.2mm
- temperature 190C (PLA)
- print speed 30mm/s outer shell, 60mm/s inner shell, 100mm/s travel
- no infill
- print acceleration 800mm/s^2, travel acceleration 1600mm/s^2
- the objects "back" side was facing to the back of the printer (towards Y MIN), so it was prone to "sagging" on the lower layers (true for any spherical object).

## Test Results
- the stock fan duct - a lot of sagging on the back, and left/right sides of the model (terrible)
- the "Semi-Circular" fan duct -  some sagging on the back, and just 1-2 layers on one of the sides
- "Spriya" design (this fan duct) - (almost) no sagging at all, with just two tiny irregularities - two "dots" on the back side of the model.
Check out the photos to see a comparison.
I have also tested with 3D Benchy - placed so the bow of the ship is facing to the right. The results were consistent, and in fact even better: sagging on bow with the stock fan duct (and some layer irregularities); just a little sagging with the semi-circular design; and _absolutely_ no sagging or defects with "Spriya".

## Tuning your heater using PID Auto Tune
This step is very important, so don't blame the fan duct if you have not performed it and you are getting bad results :)
Every time you install a new fan duct, it's highly recommended to perform the so-called "PID Auto Tune" procedure on your heat block. It's performed very easily - when your printer is cold, connect it to your PC, and start  your preferred program that allows you to send GCODE commands directly to your printer. You can use Pronterface (http://www.pronterface.com/) or whatever you prefer (some slicers have this function built in).
Next, decide what's your preferred print temperature - in my case it's 190C.
You will also have to turn the fan ON.
Execute the following GCODE command to turn it on:

    M106 S255 

Now run the PID auto tune GCODE command (replace "190" with whatever temperature you use):

    M303 E0 S190 C8

It will respond with
    Info:PID Autotune start

Your printer will go through 8 cycles of heating / cooling, so it will take a couple of minutes.
In the end, you will get a response, that looks something like this:

    bias: 164 d: 90 min: 188.73 max: 191.76
    Ku: 37.70 Tu: 20.50
    Classic PID
    Kp: 22.62
    Ki: 2.21
    Kd: 57.96
    Info:PID Autotune finished ! Place the Kp, Ki and Kd constants in the Configuration.h or EEPROM

Copy this response somewhere, otherwise it will quickly get lost in the idle "wait" messages.
Now go to your printer's Configuration -> Extruder menu, and set the control type to PID. Next, set the P value to match the Kp, I to match Ki, and D to match Kd.
Finally, save your settings to the EEPROM.
This is how it's done with the Repetier firmware. It should be almost the same with the stock Anet firmware, and similar with Skynet3D (Marlin)
When you have tuned your heater properly, the temperature will fluctuate by just plus/minus 0.5 - 1C and will be very stable.

## Notes
- Actually try it before starting to philosophize whether it's effective or not :)
- Read below to find out the recommended print settings
- To mount the fan duct, you have to loosen the screws of your fan a bit, insert the duct, and tighten them again. 
- The name "Spriya" is a Bulgarian dialect word, and means "strong wind", "vortex", "blizzard" etc ;) It's a joke.
- Check out some of my other open source stuff at http://lokspace.eu
- If you have problems slicing / printing the normal version, try the "thick walls" version - "Spriya - Anet A8 Fan Duct - Thick Walls.stl" found in the archive (it has 1.2mm walls, but it's still lightweight).
### Don't forget to post "I made one" photos! Your input is welcome!

# Print Settings

Printer: Anet A8
Rafts: No
Supports: Yes
Resolution: 0.2
Infill: 100

Notes: 
I used Cura 2.4 to print this.
Check the pictures to see how you should lay the model down when printing (rotate it 90 degrees along the X axis, and allow supports - as shown on the fifth picture. Only the attachment part needs a bit of support, so you can set the "Support Overhang Angle" to something high like 80.
You also need to set the support placement to "everywhere", and support type to "concentric".
Support density of 15% works OK. Be careful when removing the supports afterwards!
Without support, the upper part of the attachment might sag a bit, but it will still work.