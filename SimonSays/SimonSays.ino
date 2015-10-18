/*Simon Says game. Now with sound effects. 

Originaly made by Robert Spann
Code trimmed and sound effects added by digimike

Buttons are to be set on there designated pins without pull down resistors
and connected to ground rather then +5. 
*/
#include <Wire.h>

#define START 1
#define SETUP -1
#define STANDBY 0

int button[] = {2,3,4,5}; //The four button input pins
int ledpin[] = {8,9,10,11};  // LED pins
int turn = 0;  // turn counter
int buttonstate = 0;  // button state checker
int randomArray[4]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[4];  
int nextData = 0;
int state = 0;
char indicator = 0b00000000;
int fail = 0;
bool redI = false;
bool greenI = false;
void setup() 
{
  Wire.begin(14);  
  Serial.begin(9600);
  Wire.onReceive(receiveEvent); // register event
  while (!Serial);
  Serial.println("Simon Says"); 
  for(int x=0; x<4; x++)  // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
  }
  
  for(int x=0; x<4; x++) 
  {
    pinMode(button[x], INPUT);  // button pins are inputs
    digitalWrite(button[x], HIGH);  // enable internal pullup; buttons start in high position; logic reversed
  }

  randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function
  //start(); //REMOVE TO ACTUALLY HAVE IT WORK
}

void receiveEvent(int howMany) {
  Serial.println("Recieved Event");
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    if(nextData == 1) {
      Wire.flush();
      nextData = 0;
      indicator = c;
      redI = (0x10000000 & indicator) == (0x10000000);
      greenI = (0x01000000 & indicator) == (0x01000000);
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
  int result = start();
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

int start() {
  for (int y = 0; y < 4; y++)
    {
       
      digitalWrite(ledpin[0], LOW);
      digitalWrite(ledpin[1], LOW);
      digitalWrite(ledpin[2], LOW);
      digitalWrite(ledpin[3], LOW);
      delay(1000);
    
      for (int y=turn; y <= turn; y++)
      { //Limited by the turn variable
        Serial.println(""); //Some serial output to follow along
        Serial.print("Turn: ");
        Serial.print(y);
        Serial.println("");
        randomArray[y] = random(1, 5); //Assigning a random number (1-4) to the randomArray[y], y being the turn count
        for (int x=0; x <= turn; x++)
        {
          Serial.print(randomArray[x]);
        
          for(int y=0; y<4; y++)
          {
        
            if (randomArray[x] == 1 && ledpin[y] == 8) 
            {  //if statements to display the stored values in the array
              digitalWrite(ledpin[y], HIGH);
              delay(400);
              digitalWrite(ledpin[y], LOW);
              delay(100);
            }
  
            if (randomArray[x] == 2 && ledpin[y] == 9) 
            {
              digitalWrite(ledpin[y], HIGH);
              delay(400);
              digitalWrite(ledpin[y], LOW);
              delay(100);
            }
    
            if (randomArray[x] == 3 && ledpin[y] == 10) 
            {
              digitalWrite(ledpin[y], HIGH);
              delay(400);
              digitalWrite(ledpin[y], LOW);
              delay(100);
            }
  
            if (randomArray[x] == 4 && ledpin[y] == 11) 
            {
              digitalWrite(ledpin[y], HIGH);
              delay(400);
              digitalWrite(ledpin[y], LOW);
              delay(100);
            }
          }
        }
      }
      input();
      if(fail == 1) {
        fail = 0;
        turn = 0;
        return -1;
      }
    }  
   return 1;
}

void loop() 
{   
  if(state == SETUP) {
    preset();
  }  
}
 
 
 
void input() { //Function for allowing user input and checking input against the generated array
  int translatedOrder[4] = {2,3,4,5}; //R Y G B
  if(redL && !greenL) {
    translatedOrder = [5,4,2,3];
  }
  else if(!redL && !greenL) {
    translatedOrder = [4,2,3,5];
  }
  else {
    translatedOrder = [3,4,5,2];
  }
  for (int x=0; x <= turn;)
  { //Statement controlled by turn count

    for(int y=0; y<4; y++)
    {
      
      buttonstate = digitalRead(button[y]);
      
      if (buttonstate == LOW && button[y] == translatedOrder[y])
      { //Checking for button push
        digitalWrite(ledpin[0], HIGH);
        delay(200);
        digitalWrite(ledpin[0], LOW);
        inputArray[x] = 1;
        delay(250);
        Serial.print(" ");
        Serial.print(1);
        if (inputArray[x] != randomArray[x]) { //Checks value input by user and checks it against
          fail = 1;
          return;//the value in the same spot on the generated array
        }                                      //The fail function is called if it does not match
        x++;
      }
       if (buttonstate == LOW && button[y] == translatedOrder[y])
      {
        digitalWrite(ledpin[1], HIGH);
        delay(200);
        digitalWrite(ledpin[1], LOW);
        inputArray[x] = 2;
        delay(250);
        Serial.print(" ");
        Serial.print(2);
        if (inputArray[x] != randomArray[x]) {
          fail = 1;
          return;
        }
        x++;
      }

      if (buttonstate == LOW && button[y] == translatedOrder[y])
      {
        digitalWrite(ledpin[2], HIGH);
        delay(200);
        digitalWrite(ledpin[2], LOW);
        inputArray[x] = 3;
        delay(250);
        Serial.print(" ");
        Serial.print(3);
        if (inputArray[x] != randomArray[x]) {
          fail = 1;
          return;
        }
        x++;
      }

      if (buttonstate == LOW && button[y] == translatedOrder[y])
      {
        digitalWrite(ledpin[3], HIGH);
        delay(200);
        digitalWrite(ledpin[3], LOW);
        inputArray[x] = 4;
        delay(250);
        Serial.print(" ");
        Serial.print(4);
        if (inputArray[x] != randomArray[x]) 
        {
          fail = 1;
          return;
        }
        x++;
      }
    }
  }
  delay(500);
  turn++; //Increments the turn count, also the last action before starting the output function over again
}




