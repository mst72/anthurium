
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <DHT_U.h>

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE);

int mSensorPin = A1;    // select the input pin for the potentiometer
int ledPin = 7;
int ledR = 11;
int ledG = 9;
int ledB = 8;
int beepPin = 12;

int mSensorValue = 0;  // variable to store the value coming from the sensor
int onOff = HIGH;
int beepOn = LOW;
uint32_t delayMS;
long previousMillis = 0;
long interval = 120000;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);  
  pinMode(ledB, OUTPUT);
  pinMode(beepPin, OUTPUT);
  dht.begin();
  Serial.begin(9600);  
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.print  ("MinDelay:     "); Serial.println(sensor.min_delay/1000); 
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.print  ("MinDelay:     "); Serial.println(sensor.min_delay/1000); 
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  // read the value from the sensor:
  mSensorValue = analogRead(mSensorPin);    

  // Reading temperature or humidity takes about 250 milliseconds!
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
  Serial.print("M-sensor: ");                       
  Serial.println(mSensorValue);                   
  int doIt = LOW;
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
      // save the last time you blinked the LED 
      previousMillis = currentMillis;
      doIt = HIGH;   
  } else if (currentMillis < previousMillis) {
      previousMillis = currentMillis;   
  }
  else {
  }
    
  if (mSensorValue < 210) {
    // red condition
    digitalWrite(ledR, onOff);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(ledG, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(ledB, LOW);   // turn the LED on (HIGH is the voltage level)
    if (doIt == HIGH) {
      beepOn = HIGH;
    }
  } else if (mSensorValue < 350) {
    // yellow condition
    digitalWrite(ledR, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(ledG, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(ledB, onOff);   // turn the LED on (HIGH is the voltage level)
  }
  else {
    // green condition
    digitalWrite(ledR, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(ledG, onOff);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(ledB, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  digitalWrite(ledPin, onOff);   // turn the LED on (HIGH is the voltage level)
  if (onOff == HIGH) {
    if (beepOn) {
        tone(beepPin, 1000);
        beepOn = LOW;
    }
    //tone(beepPin, 1000);
    onOff = LOW;
  }
  else {
    noTone(beepPin);
    onOff = HIGH;
  }
  delay(delayMS);          
}

