#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"
#define PIN_MQ2 A3
#define DHTPIN 2
#define RED 3
#define YEL 4
#define GRN 5

DHT dht(DHTPIN, DHT22); //Инициация датчика
//DHT dht(DHTPIN, DHT11);

BH1750 lightMeter;

void setup() {
  Serial.begin(9600);  // Начать передачу данных через Serial порт
  Wire.begin();
  lightMeter.begin();
  dht.begin();
  
  pinMode(RED, OUTPUT);
  pinMode(YEL, OUTPUT);
  pinMode(GRN, OUTPUT);
  
  pinMode(PIN_MQ2, INPUT);
  
}

void loop() {
  int gas = analogRead(PIN_MQ2);
  int lux = lightMeter.readLightLevel();
  int h = dht.readHumidity(); //Измеряем влажность
  int t = dht.readTemperature(); //Измеряем температуру
  Serial.print(t);
  Serial.print("_");
  Serial.print(h);
  Serial.print("_");
  Serial.print(lux);
  Serial.print("_");
  Serial.print(gas); 
  Serial.print("_");
  

    if (t < 10){
    digitalWrite(GRN, LOW);
    digitalWrite(YEL, LOW);
    digitalWrite(RED, HIGH);
    Serial.print("0_0_1");
  }
  else if(t > 18){
    digitalWrite(RED, LOW);
    digitalWrite(YEL, LOW);
    digitalWrite(GRN, HIGH);
    Serial.print("1_0_0");
  }
  else{
    digitalWrite(RED, LOW);
    digitalWrite(GRN, LOW);
    digitalWrite(YEL, HIGH);
    Serial.print("0_1_0");
  }
  Serial.println();
  delay(100);

}
