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


https://www.thingiverse.com/thing:2392856
Motor on a roller blind by pgote is licensed under the Creative Commons - Attribution license.
http://creativecommons.org/licenses/by/3.0/

# Summary

#What is it?
I've put a motor on my roller blinds and made them controllable via MQTT messages or via a web client (two different ways to control the roller blind) that interacts with an on device embedded webserver. I've used the same sized prints for multiple different brands of blinds. 

Look at the pictures and animations that will show how it is all fitted together.

I used to have two code bases for an MQTT version and Web client version but I've merged those together into one code base and the use of MQTT is optional. It is set upon WIFI configuration - simply leave the MQTT server field blank.
  
The current version is for a ceiling mounted roller blind as shown here: https://youtu.be/Dka4of30YOY
How the pieces are assembled: https://youtu.be/3odwHns21Ik

There is a remix made for wall mounted roller blinds made by bmscott. It can be found here https://www.thingiverse.com/thing:2530155


#Additional components
DISCLAIMER: The below description is the parts that I used and how I've assembled it. If you want to use other ESP8266 boards or components it is up to you to test it :-)
- 1pc. 608ZZ bearing (8x22x7mm)
- 1pc. tiny screw for center pin fastening the center pin
- 1pc. 28BYJ-48 stepper motor + ULN2003 driver board. I'm using the 5V version of the motor (but the 12V should give better torque - see my comment below)
- 1pc. 9v power adapter
- 1pc. NodeMCU v3 Dev board
- 2pc. short M4 screws for mounting the stepper motor

Here are examples (including 2 versions of the stepper motor) of items for purchase https://goo.gl/gbb8xT

#Hook it up
Connect 9v directly to Vin on NodeMCU board (validate first that your version has a built in regulator for 9V) and to the ULN2003 board. 5V is simply not enough to be able to control the blinds, even with the higher gear ratio, so ULN2003 requires 9V. I'm using the 5V version of the stepper motor and it has worked well for me so far - there's an 12v version though that should be able to give a higher torque than the 5v running on 9v. My roller blind is just 100cm wide and if you have a really wide roller blind I would probably go for the 12V version of the stepper motor. NOTE: A battery will not last long so get a 9V power adapter.
D1 = IN1, D2 = IN3, D3 = IN2, D4 = IN4
![Wiring](https://user-images.githubusercontent.com/2181965/31534204-3ed46c28-aff6-11e7-8d02-b6090957b2de.png)
![Example](https://user-images.githubusercontent.com/2181965/31534343-cc9c85d6-aff6-11e7-9d40-615ef29b43d6.jpg)


#Flash the ESP8266-12E
- Flash your NodeMCU board with code from the following repository: https://github.com/nidayand/motor-on-roller-blind-ws 
- Follow the instructions as described in the Github repository on how to control the device.
- If you don't have an MQTT server you can use a public one during a testing (!) phase. E.g. iot.eclipse.org:1883 as described here https://iot.eclipse.org/getting-started or simply just just the web client
- Below are examples of the responsive page served from the device  - Android and iPhone UI
- You will be notified if and when new updates of the code have been released.

![Control](https://user-images.githubusercontent.com/2181965/31534387-00936ba2-aff7-11e7-8064-bc02d2b2d767.jpg)

#Access the web page
Post flashing the device:
1. Connect to the WIFI Access Point `BlindsConnectAP` and use `nidayand` as the password
1. Enter your WIFI credentials, a unique name for the device (to be used for mDNS) and, optionally, MQTT server details (leave blank if you don't want to use it)
1. The device will reboot and connect to your WIFI upon save
1. Use a web browser to connect to the device on port 80. E.g. http://192.168.2.226 or http://{configured_name}.local if you are using an mDNS supported device/client such as iOS or OSX

#You want to customize it?
I've thought of doing a wall-mounted version but I don't have that need myself so instead I can share the source code. I've used Autodesk Fusion 360 in the design and you are welcome to download and modify it further as long as you give credits to the original. Download: https://goo.gl/zsPTnC

----------

#Credits
- Ideation from http://www.instructables.com/id/Motorized-WiFi-IKEA-Roller-Blind/ . I wanted a different mount and to get it to work better an increased gear ratio is required and an higher powering voltage to be able to pull the blinds up
- 24BYJ-48 stepper CAD component. I've done some changes to the original as some distances where off -
 https://gallery.autodesk.com/fusion360/projects/86268/stepper-motor-28byj-48?searched= 
- @Lars Christensen - it is my first Fusion360 design and I used his YouTube channel to learn the basics https://www.youtube.com/user/cadcamstuff/featured
- Power options for NodeMCU boards - http://henrysbench.capnfatz.com/henrys-bench/arduino-projects-tips-and-more/powering-the-esp-12e-nodemcu-development-board/

# Print Settings

Printer: Velleman k8200
Resolution: 0.2
Infill: 30%

Notes: 
I've printed all parts in ABS and I'm using a raft only to avoid curling of the gears - otherwise I don't think it is needed.

I've used support only on the blind piece as some infill is needed for where the bearing will be.