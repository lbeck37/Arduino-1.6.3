% basiclauncher.bas, Beck 6/1/18a
!!
===========================================================
  NEW: RFO CODE FINDER
===========================================================
Press Alt+F2 in Notepad++ to display the Code Finder in
order to list and jump to your user functions and labels.

===========================================================
======= IMPORTANT: RFO-BASIC! LAUNCHER INSTRUCTIONS =======
===========================================================
Edit your program with this editor, save it somewhere on
your computer, then hit Shift+F5 to transfer and run it.
You can also double-click on any .bas file and hit Shift+F5

There are 2 ways you can choose to send program to Android:

===========================================================
    1)    WIFI MODE
===========================================================
Install the BASIC! Launcher WiFi app on your Android device:
https://play.google.com/store/apps/details?id=com.rfo.BASICLauncher
Then run it with WiFi activated.

You need your computer and your Android phone/tablet to be
on the same LAN (typically your home internet box) in order
for them to auto-detect one another.

===========================================================
    2)    USB MODE
===========================================================
Activate the "USB debugging" mode on your device: starting
with Android 4.2, go to Settings > About phone and tap 7
times on "Build number" to activate the Developer Options.
(In Android versions before 4.2, the Developer menu is
already at the root of the Settings screen).
Go to Developer Options and activate Android Debugging.

Your device needs to be perfectly recognized by your PC
= you need the original manufacturer drivers installed.
Most of the time it means to install the manufacturer
PC suite (Samsung Kies, Sony PC Companion, etc.)
!!

REM       This is the test program, try to sync it 
REM       on your Android by hitting Shift+F5...

Print "basiclauncher.bas, Beck 6/2/18b"
print "Congratulations to The Dude!"
PRINT "BASIC! Launcher test program correctly transfered"
PRINT ""
PRINT "Press the back key to exit"
DO
  PAUSE 100
UNTIL 0

OnError:
EXIT
