/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

int leds[] = {7,8,9};
int ledR = 9;
int index = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  for (int i = 0; i < 3; i++) {
    pinMode(leds[i], OUTPUT);
  }
  Serial.begin(9600);  
}

// the loop function runs over and over again forever
void loop() {
  index += 1;
  index %= 3;
  Serial.print  ("Index:   "); Serial.print  (index);
  digitalWrite(leds[index], HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  Serial.println  ("...off");
  digitalWrite(leds[index], LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  Serial.println  ("...done");
}
