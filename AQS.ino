#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring"
#define BLYNK_AUTH_TOKEN ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

char ssid[] = "";        
char pass[] = "";             

#define DHTPIN 2              
#define DHTTYPE DHT11
#define MQ135_PIN A0          
#define TEMP_THRESHOLD 30.0 

DHT dht(DHTPIN, DHTTYPE);

// Timer for Blynk
BlynkTimer timer;

void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int mqValue = analogRead(MQ135_PIN);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C  MQ135 Value: ");
  Serial.println(mqValue);

  Blynk.virtualWrite(V1, h);       
  Blynk.virtualWrite(V2, t);      
  Blynk.virtualWrite(V3, mqValue); 

 
  if (mqValue > 700) {
    Blynk.virtualWrite(V4, 1); 
    Serial.println("High MQ135 value detected! Alert sent.");
  }

  if (t > TEMP_THRESHOLD) {
    Blynk.virtualWrite(V5, 1); 
    Serial.println("High temperature detected! Alert sent.");
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  
  
  timer.setInterval(5000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run(); 
