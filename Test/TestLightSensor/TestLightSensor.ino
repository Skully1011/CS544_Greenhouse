#include <Wire.h>

byte i2c_rcv;               // data received from I2C bus
unsigned long time_start;   // start time in milliseconds
int stat_LED;               // status of LED: 1 = ON, 0 = OFF
byte value_pot;             // potentiometer position

void setup(){
	Wire.begin(); // join I2C bus as the master
	
	// initialize global variables
	i2c_rcv = 255;
	time_start = millis();
	stat_LED = 0;
	
	pinMode(13, OUTPUT);    // set pin 13 as an output
}

void loop(){
	// read potentiometer position
	value_pot = analogRead(A0);   // read the voltage at pin A0 (potentiometer voltage)

	// send potentiometer position to Slave device 0x08
	Wire.beginTransmission(0x08); // informs the bus that we will be sending data to slave device 8 (0x08)
	Wire.write(value_pot);        // send value_pot
	Wire.endTransmission();       // informs the bus and the slave device that we have finished sending data

	Wire.requestFrom(0x08, 1);    // request potentiometer position from slave 0x08
	if(Wire.available()) {	      // read response from slave 0x08
		i2c_rcv = Wire.read();
	}
	
	// blink logic code
	if((millis() - time_start) > (1000 * (float)(i2c_rcv/255))) {
		stat_LED = !stat_LED;
		time_start = millis();
	}
	digitalWrite(13, stat_LED);
}
