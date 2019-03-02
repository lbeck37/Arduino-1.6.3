#Name: Vary Temperature with Height
#Info: Adjust the temperature to assess optimal print temp for a given filament
#Depend: GCode
#Type: postprocess
#Param: startTemperature(float:220) Initial nozzle temperature
#Param: heightIncrement(float:10) Adjust temperature each time height changes by this much
#Param: tempIncrement(float:4) Decrease temperature by this much with each height increment


import re

def getValue(line, key, default = None):
	if not key in line or (';' in line and line.find(key) > line.find(';')):
		return default
	subPart = line[line.find(key) + 1:]
	m = re.search('^[0-9]+\.?[0-9]*', subPart)
	if m is None:
		return default
	try:
		return float(m.group(0))
	except:
		return default

"""
filename= "Switch.gcode"
"""

with open(filename, "r") as f:
	lines = f.readlines()

z = 0
currentTemp = startTemperature
newTemp = 0.0
started = 0

"""
filename= "Switch.gcode.txt"
"""

with open(filename, "w") as f:
	for line in lines:
		if "LAYER:0" in line:
			started = 1
		if line.startswith(';') or not started:
			f.write(line)
			continue
		myValue = getValue(line, 'G', None)
		if  myValue != None:
			newZ = getValue(line, 'Z', z)
			if newZ != z:
				z = newZ
				newTemp = float(int(z/heightIncrement)*int(tempIncrement))
				newTemp = startTemperature-newTemp
				if ( newTemp < currentTemp):
					currentTemp  = newTemp
					f.write(";TYPE:CUSTOM\n")
					f.write("M104 S" + str(newTemp) + "\n")
		f.write(line)
