
#include <Wire.h>

#define L1 12
#define L2 11
#define L3 10
#define L4 9
#define L5 8

#define S1 7
#define S2 6
#define S3 5
#define S4 4

int count= 0;
int state1 =0;
int state2 =0;
int state3 =0;
int state4 =0;

int tock=0;

#define START 1
#define SETUP -1
#define STANDBY 0

int state = 0;
int incoming = 0;
int nextData = 0;

void setup() {
  Wire.begin(15);    
  // put your setup code here, to run once:
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(L5, OUTPUT);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  Wire.onReceive(receiveEvent);
  randomSeed(analogRead(0));

  state1 = random(0,2);
  state2 = random(0,2);
  state3 = random(0,2);
  state4 = random(0,2);
  digitalWrite(L5, LOW);

}

void receiveEvent(int howMany) {
  Serial.println("Recieved Event");
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    if(nextData == 1) {
      Wire.flush();
      nextData = 0;
      Serial.println((int) c);
    }
    else if(c == 0x02) {
      state = START;
    }
    else if (c == 0x01) {
      state = SETUP;
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
  while(state == START) {
    int result = start();
    if(result == -1) {
      state = STANDBY;
      Wire.beginTransmission(4); 
      Wire.write('L');  
      Wire.endTransmission(); 
      return;
    }    
  }

}

int start() {
  digitalWrite(L1, state1);
  digitalWrite(L2, state2);
  digitalWrite(L3, state3);
  digitalWrite(L4, state4);

  delay(0.1);

  count++;
  
  if(count == 300){
    if(tock){
      tock =0;
    }else{
      tock =1;

      if(digitalRead(S1) == state1 | digitalRead(S2) == state2 | digitalRead(S3) == state3 | digitalRead(S4) == state4){
        digitalWrite(L5, HIGH);
      }else{
        digitalWrite(L5, LOW);
        return -1;
      }

      
      state1 = random(0,2);
      state2 = random(0,2);
      state3 = random(0,2);
      state4 = random(0,2);
    }
  }  
}

void loop() {
  if(state == SETUP) {
    preset();
  }
 // digitalWrite(L1, 1);


    
}
