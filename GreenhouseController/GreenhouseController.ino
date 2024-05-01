/*
This file serves as the main program for controlling the automated greenhouse and will integrate
the digital humidity/temperature sensors, light sensors, soil moisture sensors, and water level sensor
for data acquisition.

It will also control the watering pumps for the plants along with controlling the LED grow lights.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

#include "DHT.h"

// digital pins
#define RED_LED_PIN 2
#define UV_LED_PIN 3

// analog pins
#define DHT_SENSOR A0
#define LIGHT_SENSOR A1
#define WATER_SENSOR A2
#define SOIL_SENSOR A3

// misc.
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define LED_OFF 0
#define LED_ON 255

struct SensorData{
  int lightLevel;
  float humidity;
  float temperature;
  int soilMoisture;
  float waterLevel;
};

// globals
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows
SensorData sensorData;
DHT dht(DHT_SENSOR, DHTTYPE);
byte i2c_rcv = 0b0;

// function prototypes
// just printing sensorData
void displayInfoLcd();
// doesn't return anything but modifies a global struct
void gatherSensorData();
String translateLightLevel();
void initLeds();
void setLeds(int ledState);


void setup() {
  // I2C bus
  //Wire.begin();

  // LCD screen
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.setCursor(0, 0);
  // debug
  lcd.print("Hello World!");

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

void setLeds(int ledState){
  analogWrite(RED_LED_PIN, ledState);
  analogWrite(UV_LED_PIN, ledState);
}

void gatherSensorData(){
  sensorData.lightLevel = analogRead(LIGHT_SENSOR);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  sensorData.humidity = h;
  sensorData.temperature = t;

  sensorData.soilMoisture = analogRead(SOIL_SENSOR);
}

String translateLightLevel(){
  //char lightStr[7] = "";
  String lightStr;

  if (sensorData.lightLevel < 100) {
      Serial.println("Very bright");
      lightStr = "Bright";
    } else if (sensorData.lightLevel < 200) {
      Serial.println(" - Bright");
      lightStr = "Bright";
    } else if (sensorData.lightLevel < 500) {
      Serial.println(" - Light");
      lightStr = "Light";
    } else if (sensorData.lightLevel < 800) {
      Serial.println(" - Dim");
      lightStr = "Dim";
    } else {
      Serial.println(" - Dark");
      lightStr = "Dark";
    }

  
  return lightStr;
}

void displayInfoLcd(){
  String lightStr = "";
  // display sensor data on each row
  lcd.clear();
  lcd.setCursor(0, 0);
  lightStr = translateLightLevel();
  lcd.print("Light level: ");
  lcd.print(lightStr);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(String(sensorData.humidity, 1));
  lcd.print('%');
  lcd.setCursor(0, 2);
  lcd.print("Temperature ");
  lcd.print(String(sensorData.temperature, 1));
  lcd.print(" *C");
  lcd.setCursor(0, 3);
  lcd.print("Soil moisture: ");
  lcd.print(String(sensorData.soilMoisture, 10));
  delay(1000);

  // display water level?
  //delay(10000);
  // lcd.clear();
  // lcd.home();
  // lcd.print("Water level: %f", sensorData.waterLevel);

}
