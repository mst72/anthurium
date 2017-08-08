#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
int ledG = 9;

void print_it(const char * str) {
  for (int i = 0; str[i]; i++) {
    lcd.print(str[i]);  
  }  
}

void setup()
{
  pinMode(ledG, OUTPUT);  
  digitalWrite(ledG, HIGH);   
  // initialize the LCD
  lcd.begin();
  lcd.clear();
  lcd.cursor();
  lcd.leftToRight();
  lcd.home();
  lcd.setCursor(0, 0);
  // Turn on the blacklight and print a message.
  //lcd.backlight();
  //lcd.noAutoscroll();
  //lcd.autoscroll();
  print_it("Hello world");
  //lcd.print("Hello, world!");
  //lcd.print("Hello, world!");
}

void loop()
{
  digitalWrite(ledG, LOW);   
  delay(500);
  digitalWrite(ledG, HIGH);   
  delay(500);
  /*
  // Do nothing here...
  // set the cursor to (0,0):
  lcd.setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print(thisChar);
    delay(500);
  }

  // set the cursor to (16,1):
  lcd.setCursor(16, 1);
  // set the display to automatically scroll:
  lcd.autoscroll();
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print(thisChar);
    delay(500);
  }
  // turn off automatic scrolling
  lcd.noAutoscroll();

  // clear screen for the next loop:
  lcd.clear();
  */
}

