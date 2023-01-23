// Include the Software Serial library 
#include <SoftwareSerial.h>
// Define a Software Serial object and the used pins 
SoftwareSerial softSerial(8, 9);  // RX, TX 
void setup()  {
  Serial.begin(9600); 
  softSerial.begin(9600);
} 
void loop()  { 
// Check for received characters from the computer 
  if (Serial.available())  { 
// Write what is received to the soft serial
    softSerial.write(Serial.read()); 
  }else{
      //Serial.println("NotAvailable");
  }
}
