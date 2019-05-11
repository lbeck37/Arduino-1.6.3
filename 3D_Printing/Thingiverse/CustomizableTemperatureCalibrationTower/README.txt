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


https://www.thingiverse.com/thing:1298948
Customizable Temperature Calibration Tower by kerog777 is licensed under the Creative Commons - Attribution license.
http://creativecommons.org/licenses/by/3.0/

# Summary

Use this piece to test each roll of filament you get.  With the included Cura plugin, you can automatically adjust the extruder temperature with height to match the markings.  The embossed text will show what temperature each section was printed at, while also serving as a test of the effect of temperature on small overhangs.  

Note that the original Cura Plugin (now with VaryTempWithHeightOldCura.py) works the old Cura (last version = 15.06?) and the new plugin, updated by fchorney (thanks!), works with Cura 3.  For in-between versions I'm not sure what happens.  The plugin source from fchorney is at:  https://github.com/DJSymBiotiX/CuraPlugins/blob/master/post_processing_scripts/VaryTempWithHeight.py

On my Mac, I installed the plugin by putting the .py file in the following directory:
/Applications/Cura.app/Contents/Resources/plugins/plugins/PostProcessingPlugin/scripts
You might need to replace "Cura" with "Ultimaker Cura" if you used the default install name for the application.  In the applications folder just clicking on the Cura icon will open it; to look inside at the contents you have to hold down the control key and click on it, which brings up a context menu.  Choose "Show Package Contents" and you will be able to access the above directory through the finder.  Of course, you can always go there directly with the command line.  

On Windows, according to fchorney, the location is:  C:\Program Files\Ultimaker Cura 3.0\plugins\PostProcessingPlugin\scripts\

Important!  There are two .py files uploaded, one for the new and one for the old.  They are distinguished by a suffix at the end (e.g. Cura3 or OldCura).  You have to rename the file you download to just VaryTempWithHeight.py for it to register.

To use the plugin, in Cura go to Extensions->Post-Processing->Modify G-Code.  A window will pop up, where you should click "Add a script" and select "Vary Temperature with Height".

Updated to provide text for Printer & Filament brand, plus cutouts for better showcasing of bridging performance with temperature.

NOTE:  When you use the customizer to make your own version, please give it a name reflecting the printer & filament you are customizing for, rather than just "My Customized Temperature Calibration Tower".

# Print Settings

Printer: Felix 3.1
Rafts: Doesn't Matter
Supports: No
Resolution: 200 micron

Notes: 
Use a single wall, zero infill, and no top.  The Cura plugin will set the M104 code commands to adjust the extruder temperature to match the markings.