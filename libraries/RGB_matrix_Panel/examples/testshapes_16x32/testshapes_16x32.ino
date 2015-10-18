// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            10

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

#define START 1
#define SETUP -1
#define STANDBY 0

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
unsigned long now = millis();
char printTranslator[10];
int minutes, seconds;
int val = 0;
int state = 0; //0 is default state, 1 is starting, -1 is stop
int incoming = 0;

void setup() {
  Wire.begin(10);                // join i2c bus with address #8
  //Wire.onReceive(receiveEvent); // register event
  matrix.begin();
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  //pixels.begin(); // This initializes the NeoPixel library.   
  sprintf(printTranslator, "GAME");
  renderScreen();
}

void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    if(c == 0x02) {
      state = START;
    }
    else if (c == 0x03) {
      state = SETUP;
    }
    else if (c == 0x01) {
      state = STANDBY;
    }
  }
}

void renderScreen() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1); 
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print(printTranslator);
  //matrix.swapBuffers(false);
}

void start() {
  unsigned long start = millis();
  now = millis();
  seconds = 300-(start - now)/1000;
  minutes = seconds / 60;
  unsigned long prevSeconds = seconds;
  while((minutes > 0) || (seconds > 0))
  {
    if(state == STANDBY) {
      sprintf(printTranslator, "FIN");
      renderScreen();
      return;
    }
    now = millis();
    seconds = 300-((now - start)/1000);
    minutes = seconds / 60;
    if(prevSeconds != seconds) {
      if(minutes == 4 || (seconds%60)/10 == 4 || (seconds%60)%10 == 4) {
        digitalWrite(11, HIGH);
      }
      else {
        digitalWrite(11, LOW);
      }
      if(minutes == 1 || (seconds%60)/10 == 1 || (seconds%60)%10 == 1) {
        digitalWrite(10, HIGH);
      }
      else {
        digitalWrite(10, LOW);
      }
      if(minutes == 5 || (seconds%60)/10 == 5 || (seconds%60)%10 == 5) {
        digitalWrite(12, HIGH);
      }
      else {
        digitalWrite(12, LOW);
      }
      prevSeconds = seconds;
      sprintf(printTranslator, "%.2d:%.2d", minutes, seconds%60);
      renderScreen();
    }
  }

}

void preset() {
  while(state != START) {
    if(state == SETUP || state == STANDBY) {
      return;
    }
  }
  start();
}

void loop() {
  if(state == SETUP) {
    state = -2;
    preset();
  }
}
