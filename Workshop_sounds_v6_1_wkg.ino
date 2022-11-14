#include "Arduino.h"
#include "SoftwareSerial.h"
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

SoftwareSerial mySerial(10, 11); //Pins for TX/RX with DFPlayer
int ledPin = 9;    // LED lighting effect connected to pin 9
int busyPin = 12; //BUSY from DFPlayer connected to pin 12

void setup()
{
  Serial.begin(9600); //Serial Monitor start
  mySerial.begin(9600); //Serial connection to DFPlayer start
  pinMode(ledPin, OUTPUT); //Sets LED output pins
  pinMode(busyPin, INPUT); //Sets MP3 playing pin
  playSetup(); //Calls setup command for DFPLayer

  digitalWrite(ledPin,HIGH); // 'On' LED sequence
  delay (2000);
  digitalWrite(ledPin,LOW);  // LEDs set to off to start effects
}

void loop()
{
  int soundFile = random (1,10);       // Sound file selection
  int loopDelay = random (3000, 20000);   // Min. and max. delay between each loop
  
  Serial.print(F("Playing sound, file number: "));
  Serial.println(soundFile);
  execute_CMD(0x03, 0, soundFile);
  delay(1000); // Give the DFPlayer some time

  while (digitalRead(busyPin) == LOW) 
  {
    if (soundFile == 7) 
    {
      int i,count;
      count=275;
      for (i=0;i<count;i++) {
      digitalWrite(ledPin, HIGH); // set the LED on
      delay(random(45));
      digitalWrite(ledPin, LOW); // set the LED off
      delay(random(200));
        }
     
    // Wait for the DFPlayer to finish playing the MP3 file
    }

  Serial.print("Pausing before next loop, milliseconds: ");
  Serial.println(loopDelay);
  delay(loopDelay);
}
}
void playSetup()
{
  setVolume(30);
  delay(500);
  execute_CMD(0x07, 0, 5); //set EQ to Bass
  delay(500);
  execute_CMD(0x11, 0, 0); //stop repeat play 
  delay(500);
}

void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}


}
