#include <Wire.h>

String inputString = "";         // a string to hold incoming data
char inputBuffer[200];
boolean stringComplete = false;  // whether the string is complete


void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // print the string when a newline arrives:  
  delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    for (int i = 10; i < 18; i++) {
      Wire.beginTransmission(i);
      Wire.write(inChar);
      Wire.endTransmission();
    }
  }
  
}

void receiveEvent(int n) {
 while (Wire.available() > 0) {
   Serial.println(Wire.read());
 } 
}
