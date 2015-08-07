void setup() {
	pinMode(0, OUTPUT);
}	//setup

void loop() {
	digitalWrite(0, HIGH);
  delay(1500);
  // delay(500);
	digitalWrite(0, LOW);
	delay(500);
}	//loop
