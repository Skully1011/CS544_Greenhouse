/*
This file serves as the main program for controlling the automated greenhouse and will integrate
the digital humidity/temperature sensors, light sensors, soil moisture sensors, and water level sensor
for data acquisition.

It will also control the watering pumps for the plants along with controlling the LED grow lights.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "DHT.h"

// digital pins
#define RED_LED_PIN 2
#define UV_LED_PIN 3

// analog pins
#define DHTPIN A0
#define LIGHTSENSOR A1
#define 

// misc.
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define LED_OFF 0
#define LED_ON 255

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows

struct SensorData{
  int lightLevel;
  float humidity;
  float temperature;
  float soilMoisture;
  float waterLevel;
};

SensorData sensorData;
DHT dht(DHTPIN, DHTTYPE);

// function prototypes
// just printing sensorData
void displayInfoLcd();
// doesn't return anything but modifies a global struct
void gatherSensorData();
void initLeds();
void setLeds(int lightLevel);


void setup() {
  // I2C bus
  Wire.begin();

  // LCD screen
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.setCursor(0, 0);
  // debug
  //lcd.print("Hello World!");

  //  DHT sensor (humidity & temp.)
  dht.begin();

  // serial monitor debug
  Serial.begin(9600);

  // sensor data struct
  sensorData.lightLevel = 0;
  sensorData.humidity = 0;
  sensorData.temperature = 0;
  sensorData.soilMoisture = 0;

  // light sensor


  // LEDs
  initLeds();

}

void loop() {
  // put your main code here, to run repeatedly:
  // gather data
  gatherSensorData();
  // do processing
  // check light level and turn LEDs on or off
  if (sensorData.lightLevel > 800){
    setLeds(LED_ON);
  }
  else {
    setLeds(LED_OFF);
  }

  //check soil moisture and water plants if dry
  if (sensorData.soilMoisture < 50){
    // do thing
  }

  // do control
  // display information
  displayInfoLcd();
}

void initLeds(){
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(UV_LED_PIN, OUTPUT);
  setLeds(LED_OFF);
}

void gatherSensorData(){

}

char* translateLightValue(){
  char [7] lightStr = "";

  return lightStr;
}

void displayInfoLcd(){
  // display sensor data on each row
  lcd.setCursor(0, 0);
  lcd.print("Light level: %d", sensorData.lightLevel);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: %f", sensorData.humidity);
  lcd.setCursor(0, 2);
  lcd.print("Temperature %f", sensorData.temperature);
  lcd.setCursor(0, 3);
  lcd.print("Soil moisture: %f", sensorData.soilMoisture);

  // display water level?
  //delay(10000);
  // lcd.clear();
  // lcd.home();
  // lcd.print("Water level: %f", sensorData.waterLevel);

}
