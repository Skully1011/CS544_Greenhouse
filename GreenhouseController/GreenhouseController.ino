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
  int waterLevel;
};

// globals
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows
SensorData sensorData;
DHT dht(DHT_SENSOR, DHTTYPE);
unsigned long timeStart;
unsigned long timeSinceLastPrint;
unsigned long timeSinceLastWater;

// function prototypes
// just printing sensorData
void initLcdSensorStrings();
void displayInfoLcd();
// doesn't return anything but modifies a global struct
void gatherSensorData();
float convertCelsToFahren(float celsius);

String translateLightLevel();
String translateMoistureLevel();

void initLeds();
void setLeds(int ledState);

void pumpWater();



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
  sensorData.lightLevel = 0;
  sensorData.humidity = 0;
  sensorData.temperature = 0;
  sensorData.soilMoisture = 0;
  sensorData.waterLevel = 0;

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
  timeSinceLastWater = millis();
}

void loop() {

  // put your main code here, to run repeatedly:
  // gather data
  gatherSensorData();
  // do processing


  // do control
  // check light level and turn LEDs on or off
  if (sensorData.lightLevel > 800){
    setLeds(LED_ON);
  }
  else {
    setLeds(LED_OFF);
  }

  // water plants if necessary
  if (sensorData.soilMoisture > 450 && sensorData.waterLevel > 50){
    // if (millis() - timeSinceLastWater > 1000){
    //   pumpWater();
    //   timeSinceLastWater = millis();
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
  sensorData.waterLevel = analogRead(WATER_SENSOR);
}

String translateLightLevel(){
  String lightStr;

  if (sensorData.lightLevel < 100) {
      Serial.println("Very bright");
      lightStr = "Bright ";
    } else if (sensorData.lightLevel < 200) {
      Serial.println(" - Bright");
      lightStr = "Bright ";
    } else if (sensorData.lightLevel < 500) {
      Serial.println(" - Light");
      lightStr = "Light  ";
    } else if (sensorData.lightLevel < 800) {
      Serial.println(" - Dim");
      lightStr = "Dim    ";
    } else {
      Serial.println(" - Dark");
      lightStr = "Dark   ";
    }

  return lightStr;
}

String translateMoistureLevel(){
  String moistureStr;

  if (sensorData.soilMoisture < 300) {
      Serial.println(" - Soaked");
      moistureStr = "Soaked";
    } else if (sensorData.soilMoisture < 400) {
      Serial.println(" - Wet");
      moistureStr = " Wet  ";
    } else if (sensorData.soilMoisture < 500) {
      Serial.println(" - Damp");
      moistureStr = " Damp ";
    } else if (sensorData.soilMoisture < 600) {
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
  lcd.print(String(sensorData.humidity, 1));
  Serial.print("Humidity: ");
  Serial.print(sensorData.humidity);
  Serial.println();


  lcd.setCursor(12, 2);
  lcd.print(String(convertCelsToFahren(sensorData.temperature), 1));

  moistureStr = translateMoistureLevel();
  lcd.setCursor(14, 3);
  lcd.print(String(moistureStr));
  Serial.print("Soil moisture: ");
  Serial.print(sensorData.soilMoisture);
  Serial.println();

  if (sensorData.waterLevel < 50){
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
  Serial.print(sensorData.waterLevel);
  Serial.println();

  // display water level?
  // lcd.clear();
  // lcd.home();
  // lcd.print("Water level: ");
  // lcd.print(String(sensorData.waterLevel, 10));

}

void pumpWater(){
  // the relay which controls the pump is active LOW logic; pumps when set LOW
  digitalWrite(PUMP1, LOW);
  digitalWrite(PUMP2, LOW);
  delay(1000);
  digitalWrite(PUMP1, HIGH);
  digitalWrite(PUMP2, HIGH);
}