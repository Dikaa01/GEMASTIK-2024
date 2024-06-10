#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL6Tmi1y6R_"
#define BLYNK_TEMPLATE_NAME "Smart Plant Watering monitoring system based Iot"
#define BLYNK_AUTH_TOKEN "hAuHV7aTUiZJtgl2q88AhL-hLrEnFCuo"
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Gendhis Laras 5G"; // Nama Wifi
char pass[] = "gendhis01"; // WIFI password

#include "HTTPSRedirect.h"

// Sensor suhu udara
#include "DHT.h"
#define DPIN 14        
#define DTYPE DHT11   
DHT dht(DPIN, DTYPE);

// Sensor suhu air
#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBusPin = 13;
OneWire oneWire(oneWireBusPin);
DallasTemperature sensors(&oneWire);

// Sensor soil
//#define AOUT_PIN A0
const int AirValue = 1000;
const int WaterValue = 730;
int soilMoistureValue = 0;
int soilmoist = 0;

// Relay
#define relay 2
BLYNK_WRITE(V3) {
    digitalWrite(relay, param.asInt());
}

// LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  sensors.begin();
  lcd.init(); 
  lcd.backlight();
  pinMode(relay, OUTPUT); 
  digitalWrite(relay, LOW);
}

void loop() {
  Blynk.run();
  
  float tc = dht.readTemperature(false);  // Read temperature in C
  float tf = dht.readTemperature(true);   // Read Temperature in F
  float hu = dht.readHumidity();          // Read Humidity
  int Tc = dht.readTemperature(false);
  int Hu = dht.readHumidity();

  Serial.print("Sensor dht11 == ");
  Serial.print("Temp: ");
  Serial.print(tc);
  Serial.print(" C, ");
  Serial.print(tf);
  Serial.print(" F, Hum: ");
  Serial.print(hu);
  Serial.println("%");

  lcd.setCursor(0, 0);
  lcd.print("SU: ");
  lcd.setCursor(3, 0);
  lcd.print(Tc);
  lcd.print(" C");
  lcd.setCursor(8, 0);
  lcd.print("Hum: ");
  lcd.setCursor(12, 0);
  lcd.print(Hu);
  lcd.print("%");

  sensors.requestTemperatures();
  float celsius = sensors.getTempCByIndex(0);
  float fahrenheit = sensors.toFahrenheit(celsius);
   
  Serial.print("Sensor Dallas == ");
  Serial.print("Suhu Celsius: ");
  Serial.print(celsius);
  Serial.print("°C | Suhu Fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("°F");

  lcd.setCursor(0, 1);
  lcd.print("SA: ");
  lcd.setCursor(3, 1);
  lcd.print(celsius);

  soilMoistureValue = analogRead(12);
  soilmoist = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if (soilmoist >= 100) {
    soilmoist = 100;
  } else if (soilmoist <= 0) {
    soilmoist = 0;
  }

  Serial.print("Soil Humid =");
  Serial.println(soilmoist); 
  Serial.println(soilMoistureValue); 

  // Input ke Blynk
  Blynk.virtualWrite(V0, tc);
  Blynk.virtualWrite(V1, hu);
  Blynk.virtualWrite(V2, celsius);
  
  delay(500);
}
