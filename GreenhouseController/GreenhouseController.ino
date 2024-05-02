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
#define PUMP1 4
#define PUMP2 5

// analog pins
#define DHT_SENSOR A0
#define LIGHT_SENSOR1 A1
#define LIGHT_SENSOR2 A2
#define WATER_SENSOR A3
#define SOIL_SENSOR1 A4
#define SOIL_SENSOR2 A5


// misc.
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define LED_OFF 0
#define LED_ON 255

struct PlantSensorData{
  int soilMoisture;
  unsigned long timeSinceLastWater;
};

struct GreenhouseSensorData{
  float lightLevel;
  float humidity;
  float temperature;
  int waterLevel;
};

// globals
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows
GreenhouseSensorData greenhouseSensorData;
PlantSensorData plant1;
PlantSensorData plant2;
DHT dht(DHT_SENSOR, DHTTYPE);
unsigned long timeStart;
unsigned long timeSinceLastPrint;
unsigned long timeSinceLastWater;
float avgLightLevel;

// function prototypes
// just printing greenhouseSensorData
void initLcdSensorStrings();
void displayInfoLcd();
// doesn't return anything but modifies a global struct
void gatherSensorData();
float convertCelsToFahren(float celsius);

String translateLightLevel();
String translateMoistureLevel();

void initLeds();
void setLeds(int ledState);

void pumpWater(int pump);



void setup() {
  // LCD screen
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.setCursor(0, 0);
  initLcdSensorStrings();
  // debug
  // lcd.print("Hello World!");

  //  DHT sensor (humidity & temp.)
  dht.begin();

  // serial monitor debug
  Serial.begin(9600);

  // sensor data struct
  greenhouseSensorData.lightLevel = 0;
  greenhouseSensorData.humidity = 0;
  greenhouseSensorData.temperature = 0;
  greenhouseSensorData.waterLevel = 0;

  // LEDs
  initLeds();

  // Pumps
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  digitalWrite(PUMP1, HIGH);
  digitalWrite(PUMP2, HIGH);

  //time at start
  timeStart = millis();
  timeSinceLastPrint = millis();
  plant1.timeSinceLastWater = millis();
  plant2.timeSinceLastWater = millis();
}

void loop() {

  // put your main code here, to run repeatedly:
  // gather data
  gatherSensorData();
  // do processing


  // do control
  // check light level and turn LEDs on or off
  if (greenhouseSensorData.lightLevel > 800){
    setLeds(LED_ON);
  }
  else {
    setLeds(LED_OFF);
  }

  // water plants if necessary
  if (plant1.soilMoisture > 450 && greenhouseSensorData.waterLevel > 50){
    // if (millis() - plant1.timeSinceLastWater > 1000){
    //   pumpWater(PUMP1);
    //   plant1.timeSinceLastWater = millis();
    // }
  }

  if (plant2.soilMoisture > 450 && greenhouseSensorData.waterLevel > 50){
    // if (millis() - plant2.timeSinceLastWater > 1000){
    //   pumpWater(PUMP2);
    //   plant2.timeSinceLastWater = millis();
    // }
  }

  // display information
  // only update every 2 seconds; avoids flickering lcd
  if (millis() - timeSinceLastPrint > 2000){
    displayInfoLcd();
    timeSinceLastPrint = millis();
  }
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
  int lightLevel1 = analogRead(LIGHT_SENSOR1);
  int lightLevel2 = analogRead(LIGHT_SENSOR2);
  greenhouseSensorData.lightLevel = (lightLevel1 + lightLevel2)/2;

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  greenhouseSensorData.humidity = h;
  greenhouseSensorData.temperature = t;

  plant1.soilMoisture = analogRead(SOIL_SENSOR1);
  plant2.soilMoisture = analogRead(SOIL_SENSOR2);
  greenhouseSensorData.waterLevel = analogRead(WATER_SENSOR);
}

String translateLightLevel(){
  String lightStr;

  if (greenhouseSensorData.lightLevel < 100) {
      Serial.println("Very bright");
      lightStr = "Bright ";
    } else if (greenhouseSensorData.lightLevel < 200) {
      Serial.println(" - Bright");
      lightStr = "Bright ";
    } else if (greenhouseSensorData.lightLevel < 500) {
      Serial.println(" - Light");
      lightStr = "Light  ";
    } else if (greenhouseSensorData.lightLevel < 800) {
      Serial.println(" - Dim");
      lightStr = "Dim    ";
    } else {
      Serial.println(" - Dark");
      lightStr = "Dark   ";
    }

  return lightStr;
}

String translateMoistureLevel(int soilMoisture){
  String moistureStr;

  if (soilMoisture < 300) {
      Serial.println(" - Soaked");
      moistureStr = "Soaked";
    } else if (soilMoisture < 400) {
      Serial.println(" - Wet");
      moistureStr = " Wet  ";
    } else if (soilMoisture < 500) {
      Serial.println(" - Damp");
      moistureStr = " Damp ";
    } else if (soilMoisture < 600) {
      Serial.println(" - Dry");
      moistureStr = " Dry  ";
    } else {
      Serial.println(" - Arid");
      moistureStr = " Arid ";
    }

  return moistureStr;
}

float convertCelsToFahren(float celsius){
  float fahrenheit = (celsius * 1.8) + 32.0;
  return fahrenheit;
}

void initLcdSensorStrings(){
  // This function optimizes LCD printing so we can avoid printing
  // the entire strings 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Light level: ");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(14, 1);
  lcd.print('%');

  lcd.setCursor(0, 2);
  lcd.print("Temperature ");
  lcd.setCursor(15, 2);
  lcd.print(" *F");

  lcd.setCursor(0, 3);
  lcd.print("Soil moisture:");
}

void displayInfoLcd(){
  unsigned long lcdDisplayTime = millis();
  String lightStr = "";
  String moistureStr = "";
  // display sensor data on each row
  // lcd.clear();
  // lightStr will always start on column 13
  lightStr = translateLightLevel();
  lcd.setCursor(13, 0);
  lcd.print(lightStr);

  lcd.setCursor(10, 1);
  lcd.print(String(greenhouseSensorData.humidity, 1));
  Serial.print("Humidity: ");
  Serial.print(greenhouseSensorData.humidity);
  Serial.println();


  lcd.setCursor(12, 2);
  lcd.print(String(convertCelsToFahren(greenhouseSensorData.temperature), 1));

  moistureStr = translateMoistureLevel(plant1.soilMoisture);
  lcd.setCursor(14, 3);
  lcd.print(String(moistureStr));
  Serial.print("Soil moisture: ");
  Serial.print(plant1.soilMoisture);
  Serial.print(plant2.soilMoisture);
  Serial.println();

  if (greenhouseSensorData.waterLevel < 50){
    // wait a few seconds before displaying the warning
    while (millis() - lcdDisplayTime < 3000){};
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water level low!");
    lcd.setCursor(0, 1);
    lcd.print("Please refill");
    lcd.setCursor(0, 2);
    lcd.print("reservoir.");
    while (millis() - lcdDisplayTime < 6000){};
    initLcdSensorStrings();
  }
  Serial.print("Water level: ");
  Serial.print(greenhouseSensorData.waterLevel);
  Serial.println();

  // display water level?
  // lcd.clear();
  // lcd.home();
  // lcd.print("Water level: ");
  // lcd.print(String(greenhouseSensorData.waterLevel, 10));

}

void pumpWater(int pump){
  // the relay which controls the pump is active LOW logic; pumps when set LOW
  digitalWrite(pump, LOW);
  delay(1000);
  digitalWrite(pump, HIGH);
}