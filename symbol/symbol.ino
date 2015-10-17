
/***************************************************
  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
 
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8

// Using software SPI is really not suggested, its incredibly slow
//Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
// Use hardware SPI
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);
int level = 0;
int pb1 = 2;
int pb2 = 3;
int pb3 = 4;
int pb4 = 5;

char * key[6][7] = {
  {"a","b","c","d","e","f","g"},
  {"h","a","g","i","j","f","k"},
  {"t","s","i","o","r","c","j"},
  {"l","m","n","e","o","k","p"},
  {"q","p","n","t","m","b","a"},
  {"l","h","f","r","q","d","s"}
};
int column = 0;
void setup() {
  Serial.begin(9600);
  pinMode(pb1, INPUT);
  pinMode(pb2, INPUT);
  pinMode(pb3, INPUT);
  pinMode(pb4, INPUT);
  while (!Serial);
  randomSeed(analogRead(0));
  Serial.println("Adafruit 2.2\" SPI TFT Test!"); 
 
  tft.begin();
      tft.setRotation(1);
}

void stop() {
  
}

void prepare() {
  
}

int start() {
  while(level < 4) {
    column = random(5);
    int first = random(3);
    int second = random(first+1,5-(first+1));
    int third = random(second+1,6-(second+1));
    int fourth = random(third+1,7-(second+1));
    char * scramble[4] = {key[column][first], key[column][second], key[column][third], key[column][fourth]};
    int KEY[4] = {0,1,2,3};
    char SerialPrinter[100];
    sprintf(SerialPrinter, "[%s, %s, %s, %s]", scramble[0], scramble[1], scramble[2], scramble[3]);
    Serial.print("Column ");
    Serial.println(column);
    Serial.println(SerialPrinter);    
    for (int i=0; i<4; i++)
    {
     int r = random(i,3); // dont remember syntax just now, random from a to 8 included.
     int temp = KEY[i];
     KEY[i] = KEY[r];
     KEY[r] = temp;
    }
    displaySymbols(scramble[KEY[0]], scramble[KEY[1]], scramble[KEY[2]], scramble[KEY[3]]);
    sprintf(SerialPrinter, "KEY: %d %d %d %d", KEY[0], KEY[1], KEY[2], KEY[3]);
    Serial.println(SerialPrinter);
    int buttonState[4] = {0};
    //Button Press 1
    for(int i = 0; i < 4; i++) {
      while(buttonState[0] == LOW && buttonState[1] == LOW && buttonState[2] == LOW && buttonState[3] == LOW) {
        buttonState[0] = digitalRead(pb1);
        buttonState[1] = digitalRead(pb2);
        buttonState[2] = digitalRead(pb3);
        buttonState[3] = digitalRead(pb4);
      }
      
      sprintf(SerialPrinter, "Button Status: %d %d %d %d", buttonState[0], buttonState[1], buttonState[2], buttonState[3]);
      Serial.println(SerialPrinter);
      int pushed = -1;
      if(buttonState[0] == HIGH) {
        pushed = 0;
      }
      else if (buttonState[1] == HIGH) {
        pushed = 1;
      }
      else if (buttonState[2] == HIGH) {
        pushed = 2;
      }
      else {
        pushed = 3;
      }
      if(pushed != KEY[i]) {
        return -1;
      }
      tft.fillRect((pushed%2)*140, (pushed >=2 )?120:0 , 140, 120, ILI9340_YELLOW);      
      buttonState[0] = LOW;
      buttonState[1] = LOW;
      buttonState[2] = LOW;
      buttonState[3] = LOW;
      delay(1000);
    }

    level++;
    delay(3000);
  }
}

void loop(void) {
  start();

  
/*  delay(7500);
  displaySymbols("e","f","g","h");
  delay(7500);
  displaySymbols("i","j","k","l");
  delay(7500);
  displaySymbols("m","n","o","p");
  delay(7500);
  displaySymbols("q","r","s","t");
  delay(7500);
  displaySymbols("a","b","c","d"); */
  
}

void displaySymbols(const char * a, const char * b, const char * c, const char * d) {
  tft.fillScreen(ILI9340_BLACK);
  unsigned long start = micros();
  tft.setTextColor(ILI9340_GREEN);
  tft.setTextSize(7);
  tft.drawFastVLine(140, 0, 240, ILI9340_WHITE);
  tft.drawFastVLine(300, 0, 240, ILI9340_WHITE);
  tft.drawFastHLine(0, 120, 300, ILI9340_WHITE);
  tft.drawFastHLine(300, 80, 20, ILI9340_WHITE);
  tft.drawFastHLine(300, 160, 20, ILI9340_WHITE);
  for(int i = 0; i < level; i++) {
    tft.fillRect(300, i*80, 20, 80, ILI9340_WHITE);
    tft.drawFastHLine(300, i*80-1, 20, ILI9340_BLACK);
  }
  tft.setCursor(40, 40);
  tft.println(a);
  tft.setCursor(200, 40);
  tft.println(b);
  tft.setCursor(40, 160);
  tft.println(c);
  tft.setCursor(200, 160);
  tft.println(d);
}


unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}


