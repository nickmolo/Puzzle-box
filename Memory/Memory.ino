/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

int count = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
  lcd.setCursor(6,0);
  lcd.print("0");

  lcd.setCursor(0,1);
  lcd.print("1");

  lcd.setCursor(5,1);
  lcd.print("2");

  lcd.setCursor(10,1);
  lcd.print("3");

  lcd.setCursor(15,1);
  lcd.print("4");

  
  
  lcd.setCursor(9,0);
  lcd.print("Stage");

  lcd.setCursor(15,0);
  lcd.print("0");
  randomSeed(analogRead(0));
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  int list[] = {1,2,3,4};

  for (int a=0; a<4; a++){
    int r = random(a,4); // dont remember syntax just now, random from a to 8 included.
    int temp = list[a];
    list[a] = list[r];
    list[r] = temp;
  }
 
  lcd.setCursor(6,0);
  lcd.print(random(0,5));

  lcd.setCursor(0,1);
  lcd.print(list[0]);

  lcd.setCursor(5,1);
  lcd.print(list[1]);

  lcd.setCursor(10,1);
  lcd.print(list[2]);

  lcd.setCursor(15,1);
  lcd.print(list[3]);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  delay(1000);
}

