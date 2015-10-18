// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6
#define PIN2            8
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1
#define NUMPIXELS2     10
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel indicatorL = Adafruit_NeoPixel(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);

#define START 1
#define SETUP -1
#define STANDBY 0

int delayval = 500; // delay for half a second
int state = 0;
char indicator = 0b00000000;
int nextData = 0;
void setup() {
  Wire.begin(11);  
  Serial.begin(9600);
  Wire.onReceive(receiveEvent); // register event
  while (!Serial);
  Serial.println("Big Button"); 
  randomSeed(analogRead(0));
  pinMode(5, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  indicatorL.begin();
  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.show();
  for(int i = 0; i < NUMPIXELS2; i++) {
    indicatorL.setPixelColor(i, pixels.Color(0,0,0));
  }
  indicatorL.show();
}

void receiveEvent(int howMany) {
  Serial.println("Recieved Event");
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    if(nextData == 1) {
      Wire.flush();
      nextData = 0;
      indicator = c;
      Serial.print("Indicator is: ");
      Serial.println((int) c);
    }
    else if(c == 0x02) {
      state = START;
    }
    else if (c == 0x01) {
      state = SETUP;
      nextData = 1;
    }
    else if (c == 0x03) {
      state = STANDBY;
    }
    Serial.println(state);
  }
}

void preset() {
  while(state != START) {
    delay(1);
  }
  start();
}

void start() {

  unsigned long colors[7] = {0,0,0,0,0,0,0};
  int count = 0;

  if (0b10000000 & indicator) 
    colors[count++] = pixels.Color(150,0,0);
    
  if (0b01000000 & indicator)
    colors[count++] = pixels.Color(0,150,0);
    
  if (0b00100000 & indicator) 
    colors[count++] = pixels.Color(0,0,150);

  if (0b00010000 & indicator) 
    colors[count++] = pixels.Color(150,150,0); 

  if (0b00001000 & indicator) 
    colors[count++] = pixels.Color(150,0,150); 

  if (0b00000100 & indicator) 
    colors[count++] = pixels.Color(150,150,150); 

  if (0b00000010 & indicator) 
    colors[count++] = pixels.Color(150,50,0);

  for (int i = 0; i < 10; i++) {
    indicatorL.setPixelColor(i, colors[i%count]);
  }


  indicatorL.show();
  int result = begin();
  for(int i = 0; i < NUMPIXELS2; i++) {
    indicatorL.setPixelColor(i, pixels.Color(0,0,0));
  }
  indicatorL.show();
  if(result == -1) {
    Wire.beginTransmission(4); 
    Wire.write('L');  
    Wire.endTransmission(); 
  }
  else if (result == 1){
    Wire.beginTransmission(4); 
    Wire.write('W');  
    Wire.endTransmission(); 
  }  
}

int begin() {
  unsigned long sTime = millis();
  int activeColor = random(4);
  int require = 0; //0 is hold, 1 is press and release
  Serial.print("Active Color ");
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
      while(digitalRead(5) == LOW) {
        if(state == STANDBY) {
          return -2;
        }
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
      if(digitalRead(5) == LOW) {
        return -1;
      }
      while(digitalRead(5) == HIGH) {
        if(state == STANDBY) {
          return -2;
        }
      }
      char bufferd[20];
      sprintf(bufferd, "Pins: 2 [#5] %d, 3 [#1] %d, 4 [#4] %d", digitalRead(2), digitalRead(3), digitalRead(4));
      Serial.println(bufferd);
      if(holdColor == 0) {
        //Blue
        //CHECK FOR TIME WITH 4 
         if(digitalRead(4) == HIGH) {
          return 1;
        }
      }
      else if(holdColor == 1) {
        //Green
        //CHECK FOR TIME WITH 1
        if(digitalRead(3) == HIGH) {
          return 1;
        }        
      }
      else if(holdColor == 2) {
        //RED
        //CHECK FOR TIME WITH 5
        if(digitalRead(2) == HIGH) {
          return 1;
        }
      }
      else {
        //YELLOW WHITE MAGENTA ETC
        //CHECK FOR 1
         if(digitalRead(3) == HIGH) {
          return 1;
          }               
      }
      

      //DO TIMING CHECKING TO MAKE SURE ITS SYNCHED WITH CLOCK [assume true]
      return -1;
  }
  else {
      while(digitalRead(5) == LOW) {
        if(state == STANDBY) {
          return -2;
        }
      }
      delay(750);
      if(digitalRead(5) == HIGH) {
        return -1;
      }
      return 1;
  }
  return 1;
}


void loop() {
  if(state == SETUP) {
    preset();
  }
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

