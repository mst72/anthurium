#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <DHT_U.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTPIN 5     // what digital pin we're connected to
// conditions
#define RED 0
#define YELLOW 1
#define GREEN 2
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE);
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// analog pins
int mSensorPinA = A1;    // select the input pin for the potentiometer
int lcdSda = A4;
int lcdScl = A5;

// digital pins
int btnPin = 3;
int mSensorPinD = 4;    // digital MS output
// DHT pin == 5
int ledR = 7;
int ledY = 8;
int ledG = 9;
int beepPin = 10;
int relayPin1 = 11; // relay (sensor-1)
int relayPin2 = 12; // relay (sensor-1)
/////////////////////////////////////////////////////

int onOff = HIGH;
int beepOn = LOW;
uint32_t delayMS;
long previousMillis = 0;
long interval = 120000;
int sleep_time = 0;
long prevRelayMS = 0;
const long intervalRelayMS = 5000;
const int max_sleep_time = 10;
#define RS_SAMPLE 0
#define RS_FIRST 1
#define RS_SECOND 2
#define RS_MAX (RS_SECOND+1)

int relayState = RS_SAMPLE;
int mSensorValue[RS_MAX];  // variable to store the value coming from the sensor



void setup_pins()
{
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);  
  pinMode(ledY, OUTPUT);
  pinMode(beepPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(btnPin, INPUT);
  pinMode(mSensorPinD, INPUT);
}

void setup_dht()
{
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

void print_it(const char * str, bool reset) {
  if (reset) {
    lcd.clear();
    lcd.setCursor(0, 0);
  }
  for (int i = 0; str[i]; i++) {
    lcd.print(str[i]);  
  }  
}

void setup_lcd()
{
  // initialize the LCD
  lcd.begin();
  lcd.clear();
  //lcd.cursor();
  //lcd.leftToRight();
  lcd.home();
  lcd.setCursor(0, 0);
  // Turn on the blacklight and print a message.
  //lcd.noBacklight();
  lcd.backlight();
  //lcd.noAutoscroll();
  //lcd.autoscroll();
  print_it("Hello world!!!", false);
  //lcd.print("Hello, world!");
  //lcd.print("Hello, world!");
  update_relays();
}

void setup() {
  memset(mSensorValue, 0, sizeof(mSensorValue));
  setup_pins();
  setup_lcd();
  setup_dht();
}

int check_m_sensor_red(int value) 
{
  if (mSensorValue < 210) {
    return RED;
  } else if (mSensorValue < 350) {
    return YELLOW;
  }
  return GREEN;
}

int check_m_sensor_black(int value) 
{
  if (mSensorValue > 700) {
    return RED;
  } else if (mSensorValue > 360) {
    return YELLOW;
  }
  return GREEN;
}

int check_m_sensor(int value) 
{
  return check_m_sensor_black(value);
}


void process_relay_states()
{
  bool changeState = false;
  unsigned long currMS = millis();
  if ( currMS - prevRelayMS > intervalRelayMS ) {
      prevRelayMS = currMS;
      changeState = true;
  } else if (currMS < prevRelayMS) 
  {
      // forced reset
      prevRelayMS = currMS;   
      changeState = true;
  }
  else {
  }
  if (changeState) {
    if (sleep_time > 0) {
      sleep_time -= 1;
    }
    else {
      relayState += 1;
      if (relayState >= RS_MAX) {
        relayState = 0;
        sleep_time = max_sleep_time;
      }
      update_relays();
    }
  }
}

void update_relays()
{ 
  Serial.print("R-State: ");
  Serial.print(relayState);
  int first  = (relayState & 0x01)?HIGH:LOW;
  int second = (relayState & 0x02)?HIGH:LOW;
  digitalWrite(relayPin1, first);
  digitalWrite(relayPin2, second);
  delay(200); // anti-bounce
}

void loop() {
  char io_buff[50];
  char str_temp[6];
  memset(io_buff, 0, sizeof(io_buff));
  // Reading temperature or humidity takes about 250 milliseconds!
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  print_it("T:", true);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
    print_it("Err", false);
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
    dtostrf(event.temperature, 4, 2, str_temp);
    //lcd.print(event.temperature);
    sprintf(io_buff, "%sC", str_temp); 
    print_it(io_buff, false);
  }
  // Get humidity event and print its value.
  print_it(" H:", false);
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
    print_it("Err", false);
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
    dtostrf(event.relative_humidity, 4, 2, str_temp);
    //lcd.print(event.temperature);
    sprintf(io_buff, "%s", str_temp); 
    print_it(io_buff, false);
  }
  process_relay_states();
  // read the value from the sensor:
  mSensorValue[relayState] = analogRead(mSensorPinA);    
  // todo: digital read
  // todo: btn support
  Serial.print("M-sensor: ");                       
  for (int i = 0; i < RS_MAX; i++) {
    sprintf(io_buff, "M%d:%04d ", i, mSensorValue[i]); 
    Serial.print(io_buff);                   
  }
  sprintf(io_buff, " MD%d:%d ", relayState, digitalRead(mSensorPinD)); 
  Serial.println(io_buff);                       
  Serial.print("Btn: ");                       
  Serial.println(digitalRead(btnPin));                       
  lcd.setCursor(0, 1);
  if (sleep_time > 0) {
    Serial.print("Sleep time: ");                       
    Serial.println(sleep_time);                       
    sprintf(io_buff, "-:%04d/%04d/%04d", mSensorValue[0], mSensorValue[1], mSensorValue[2]); 
  }
  else {
    sprintf(io_buff, "%d:%04d/%04d/%04d", relayState, mSensorValue[0], mSensorValue[1], mSensorValue[2]); 
  }
  print_it(io_buff, false);
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
  switch(check_m_sensor(mSensorValue[relayState])) {
    case RED:
      // red condition
      digitalWrite(ledR, onOff);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledG, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledY, LOW);   // turn the LED on (HIGH is the voltage level)
      if (doIt == HIGH) {
        beepOn = HIGH;
      }
    break;
    case YELLOW: 
      // yellow condition
      digitalWrite(ledR, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledG, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledY, onOff);   // turn the LED on (HIGH is the voltage level)
    break;
    case GREEN: 
      // green condition
      digitalWrite(ledR, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledG, onOff);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledY, LOW);   // turn the LED on (HIGH is the voltage level)
    break;
    default: // unknown!!!
      digitalWrite(ledR, onOff);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledG, onOff);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledY, onOff);   // turn the LED on (HIGH is the voltage level)
    break;
  }  
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

