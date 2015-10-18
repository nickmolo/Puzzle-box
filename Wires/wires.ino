

#define SETUP 1
#define START 2
#define DEFAULTM 3

#include <Wire.h>

int wireMap = 0;
int lightMap = 0;
int state = DEFAULTM;

int wires[4] = {0, 0, 0, 0};
int lights[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  
  //Wires 1-4
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
  //Submit button
  pinMode(13, INPUT);
  
  //Lights
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  Wire.begin(13);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
}

void receiveEvent(int n) {
  Serial.println("Recieved Event");
  while (Wire.available() > 0) {
    char c = Wire.read();
    
    if (c == 1) {
      state = SETUP;
    } else if (c == 2) {
      state = START;
    } else {
      state = DEFAULTM;
    }
    
    if (state == SETUP) {
      int d = Wire.read();
      wireMap = Wire.read();
      lightMap = Wire.read();
      
      Serial.println(state);
      Serial.println(wireMap);
      Serial.println(lightMap);
    } else {
      Serial.flush();
    }    
  }
}

int begin() {
  
  wires[0] = (wireMap & 0b10000000) > 0;
  wires[1] = (wireMap & 0b01000000) > 0;
  wires[2] = (wireMap & 0b00100000) > 0;
  wires[3] = (wireMap & 0b00010000) > 0;
  
  lights[0] = (lightMap & 0b10000000) > 0;
  lights[1] = (lightMap & 0b01000000) > 0;
  lights[2] = (lightMap & 0b00100000) > 0;
  lights[3] = (lightMap & 0b00010000) > 0;
  lights[4] = (lightMap & 0b00001000) > 0;
  lights[5] = (lightMap & 0b00000100) > 0;
  lights[6] = (lightMap & 0b00000010) > 0;
  lights[7] = (lightMap & 0b00000001) > 0;
  
  for (int i = 0; i < 8; i++) {
    digitalWrite(i+2, lights[i]); 
  }
  
  while(1) {
    if (digitalRead(13)) {
      int win = 1;
      
      if (wires[0] != digitalRead(A0)) {
        Serial.println("1");
        win = 0;
      }
      
      if (wires[1] != digitalRead(A1)) {
        Serial.println("2");
        win = 0;
      }
      
      if (wires[2] != digitalRead(A2)) {  
        Serial.println("3");
        win = 0;
      }
      
      if (wires[3] != digitalRead(A3)) {
        Serial.println("4");
        win = 0;
      }
      
      Wire.beginTransmission(4);
      if (win) {
        Wire.write('W');
      } else {
        Wire.write('L');
      }
      Wire.endTransmission();
       
    }
    
  }
}

void preset() {
  while(state != START) {
    delay(1);
  }
  begin();
}

// the loop routine runs over and over again forever:
void loop() {
      if (state == SETUP)
        preset();
}
