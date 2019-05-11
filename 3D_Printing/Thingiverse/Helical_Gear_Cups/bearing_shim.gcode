;FLAVOR:Marlin
;TIME:0
;Filament used: 0m
;Layer height: 0.1
;Generated with Cura_SteamEngine 3.0.4
M190 S60
M104 S200
M109 S200
M82 ; absolute extrusion mode
G21 ;metric values
G90 ;absolute positioning
M82 ;set extruder to absolute mode
M107 ;start with the fan off
G28 X0 Y0 ;move X/Y to min endstops
G28 Z0 ;move Z to min endstops
G1 Z15.0 F9000 ;move the platform down 15mm
G92 E0 ;zero the extruded length
G1 F200 E3 ;extrude 3mm of feed stock
G92 E0 ;zero the extruded length again
G1 F9000
;Put printing message on LCD screen
M117 Printing...
;LAYER_COUNT:0
G1 F1500 E-6.5
M107
M104 S0 ;extruder heater off
M140 S0 ;heated bed heater off (if you have it)
G91 ;relative positioning
G1 E-1 F300  ;retract the filament a bit before lifting the nozzle, to release some of the pressure
G1 Z+0.5 E-5 X-20 Y-20 F9000 ;move Z up a bit and retract filament even more
G28 X0 Y0 ;move X/Y to min endstops, so the head is out of the way
M84 ;steppers off
G90 ;absolute positioning=
M82 ; absolute extrusion mode
M104 S0
;End of Gcode
;SETTING_3 {"global_quality": "[general]\\nversion = 2\\nname = empty\\ndefiniti
;SETTING_3 on = prusa_i3\\n\\n[metadata]\\ntype = quality_changes\\nquality_type
;SETTING_3  = normal\\n\\n[values]\\nadhesion_type = skirt\\nmaterial_bed_temper
;SETTING_3 ature = 60\\nmaterial_diameter = 1.75\\nmaterial_print_temperature = 
;SETTING_3 200\\nsupport_type = buildplate\\n\\n"}
