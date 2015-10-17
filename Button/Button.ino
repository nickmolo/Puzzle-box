// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int delayval = 500; // delay for half a second
char indicator = 0b00100000;
void setup() {
    Serial.begin(9600);
      while (!Serial);
      Serial.println("Big Button"); 
      randomSeed(analogRead(0));
  pinMode(2, INPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  int result = begin();
  if(result == -1) {
    Serial.println("You lost!");
  }
  else {
    Serial.println("You won!");
  }
}

int begin() {
  int activeColor = random(4);
  int require = 0; //0 is hold, 1 is press and release
  Serial.print("Active Color");
  Serial.println(activeColor);
  if(activeColor == 0) { //Red
      pixels.setPixelColor(0, pixels.Color(150,0,0));
  }
  else if (activeColor == 1) { //blue
      pixels.setPixelColor(0, pixels.Color(0,0,150));
  }
  else if (activeColor == 2) { //green
      pixels.setPixelColor(0, pixels.Color(0,150,0)); 
  }  
  else if (activeColor == 3) { //yellow
      pixels.setPixelColor(0, pixels.Color(150,150,0)); 
  }
  else { //magenta
      pixels.setPixelColor(0, pixels.Color(150,0,150));  
  }
  pixels.show();
  if(activeColor == 1 && (0b01000000 & indicator)) {
     require = 0;
     Serial.println("Green");
  }
  else if (activeColor == 0 && (0b00001000 & indicator)) {
    require = 1;
    Serial.println("Magenta");
  }
  else if (activeColor == 1 && ((0b10001000 & indicator) == 0b10001000)) {
    require = 0;
    Serial.println("Red and Magenta");
  }
  else if (activeColor == 2 && ((0b10000000 & indicator) || (0b00100000 & indicator))) {
    require = 1;
    Serial.println("Red or blue");
  }
  else if(activeColor == 3) {
    require = 0;
  }
  else if(0b11100000 & indicator == 0b11100000) {
    require = 1;
    Serial.println("RGB");
  }

  if(!require) { //HOLD
    int holdColor = random(5);
      while(digitalRead(2) == LOW) {
        delay(100);
      }
      delay(750);
      if(holdColor == 0) {
        //Blue
        pixels.setPixelColor(0, pixels.Color(0,0,150));
      }
      else if(holdColor == 1) {
        //Green
        pixels.setPixelColor(0, pixels.Color(0,150,0));
        
      }
      else if(holdColor == 2) {
        //RED
        pixels.setPixelColor(0, pixels.Color(150,0,0));
      }
      else if(holdColor == 3){
        //YELLOW 
        pixels.setPixelColor(0, pixels.Color(150,150,0));
      }      
      else if (holdColor == 4) {
        // WHITE
        pixels.setPixelColor(0, pixels.Color(150,150,150));
      }
      else {
        pixels.setPixelColor(0, pixels.Color(150,0,150));
      }
      pixels.show();
      if(digitalRead(2) == LOW) {
        return -1;
      }
      while(digitalRead(2) == HIGH) {
      }
      if(holdColor == 0) {
        //Blue
        //CHECK FOR TIME WITH 2 
      }
      else if(holdColor == 1) {
        //Green
        //CHECK FOR TIME WITH 4
        
      }
      else if(holdColor == 2) {
        //RED
        //CHECK FOR TIME WITH 1
      }
      else {
        //YELLOW WHITE MAGENTA ETC
        //CHECK FOR 3
        
      }
      //DO TIMING CHECKING TO MAKE SURE ITS SYNCHED WITH CLOCK [assume true]
      return 1;
  }
  else {
      while(digitalRead(2) == LOW) {
      }
      delay(750);
      if(digitalRead(2) == HIGH) {
        return -1;
      }
      return 1;
  }
  return 1;
}


void loop() {
  
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
/*  if(digitalRead(2) == HIGH) {
    pixels.setPixelColor(0, pixels.Color(150,150,150)); // Moderately bright green color.

     // This sends the updated pixel color to the hardware.
  }
  else {
    pixels.setPixelColor(0, pixels.Color(150,150,0)); // Moderately bright green color.
  }
  pixels.show(); */
}

