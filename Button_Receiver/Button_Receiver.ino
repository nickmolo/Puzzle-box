/*                Receiver
For more details see: http://projectsfromtech.blogspot.com/

Connect the Tx pin of this board to the Rx of the board running Flash_the_LED__Tx.ino
Connect the Grounds of the two boards

Receive an integer value over the serial and 
flashes the LED the appropriate number of times
*/
int val = 0;
const int led = 13;
int incoming = 0;

void setup()
{
  Serial.begin(4800);
  pinMode(led, OUTPUT);     
}

void loop()
{
  incoming = Serial.available();
  while (incoming == 0)                 //Keep checking until there is something available 
  {
    incoming = Serial.available();
  }

  val = Serial.parseInt();             //Reads integers as integer rather than ASCI. Anything else returns 0
}

