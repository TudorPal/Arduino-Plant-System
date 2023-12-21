// DHT setup
#include "DHT.h"
#define dhtPin 2
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);

// Soil sensor setup
#define soilPin A0

// Pump setup
#define pumpPin 6

// BMP setup
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp; // I2C

void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(soilPin, INPUT);
  pinMode(pumpPin, OUTPUT);

  // BMP:
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  status = bmp.begin(BMP280_ADDRESS);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
  /* Default settings from the datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  // DHT22
  float dhtHumidity,dhtTemperature;
  dhtFunction(&dhtHumidity, &dhtTemperature);
  Serial.print(F("DHT Air Humidity: "));
  Serial.print(dhtHumidity);
  Serial.print(F("%   DHT Air Temperature: "));
  Serial.print(dhtTemperature);
  Serial.print(F("°C "));

  // soil
  int soilHumidity;
  soilFunction(&soilHumidity);
  Serial.println(soilHumidity);

  // pump
  pumpFunction();

  // BMP280
  float bmpTemperature, bmpPressure, bmpAltitude;
  bmpFunction(&bmpTemperature, &bmpPressure, &bmpAltitude);
  Serial.print(F("Temperature = "));
  Serial.print(bmpTemperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmpPressure);
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmpAltitude); /* Adjusted to local forecast! */
  Serial.println(" m");

  Serial.println();  
  delay(2000);
}

void dhtFunction(float* h, float* t){
  *h = dht.readHumidity(); // humidity
  *t = dht.readTemperature();  // temperature as Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(*h) || isnan(*t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }  
}

void bmpFunction(float* t, float* pa, float* alt){
  *t = bmp.readTemperature();
  *pa = bmp.readPressure();
  *alt = bmp.readAltitude(*pa);
}

void soilFunction(int *h){
  *h = analogRead(soilPin);
}

void pumpFunction(){
  // negative logic, 1 - on, 0 - off
  Serial.println("Starting pump...");
  digitalWrite(pumpPin, 1);
  delay(1000);
  Serial.println("Stopping pump...");
  digitalWrite(pumpPin, 0);
  delay(3000);  
}