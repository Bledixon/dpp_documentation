 
#include <Bounce2.h>

// Relay Ports and Variables
#define RELAY_ON 1          // Relay ON is on
#define RELAY_OFF 0         // Relay is of, active on O
#define Relay 36            // Pin for set (S) the relay

// Motor A connections
int enA = 5;
int in1 = 22;
int in2 = 24;
// Motor B connections
int enB = 4;
int in3 = 23;
int in4 = 25;

int motorSpinTime = 2500;       //How long rotates the motor to spin up the rope

//Communication
//int incomingByte = 0;
//int commu = 44;

// Button Trigger
#define BUTTON_PIN 50
#define LED_PIN 13

int ledState = LOW;

// INSTANTIATE A Button OBJECT FROM THE Bounce2 NAMESPACE
Bounce2::Button button = Bounce2::Button();

//Coding
bool boxIsAtTop = true;
bool boxIsAtBottom = false;

void setup() {
  //boxIsAtTop = true;
  
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //pinMode(commu, INPUT);
  
  // Turn off motors - Initial state
  stopMotor();

  // set relay to closed modus --> Power to engine
  digitalWrite(Relay, RELAY_OFF);
  pinMode(Relay, OUTPUT);

  //Communication
  Serial.begin(115200);
  //Serial1.begin(115200); 

  button.attach( BUTTON_PIN, INPUT ); // USE EXTERNAL PULL-UP
  button.interval(5); 
  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button.setPressedState(LOW); 

  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,ledState);
  
  delay(1000);
}

void loop() {

  if(boxIsAtTop){
    boxIsAtTop = false;
    relayOn();
    motorTurnRight();
    delay(2000);
    relayOff();
    motorTurnLeft();
    boxIsAtBottom = true;
    delay(1000);
    Serial.println("Finished Spinning");
  }

  button.update();
  if ( button.pressed() ) {
    ledState = !ledState; // SET ledState TO THE OPPOSITE OF ledState
    digitalWrite(LED_PIN,ledState); // WRITE THE NEW ledState

         if(boxIsAtBottom){ 
        boxIsAtBottom = false;
        relayOn();
        delay(3000);
        relayOff();
        boxIsAtTop = true;
        delay(10000);
     }
    
  }

  /*
  if (digitalRead(commu) == HIGH)
  {
     if(boxIsAtBottom){ 
        boxIsAtBottom = false;
        relayOn();
        delay(3000);
        relayOff();
        boxIsAtTop = true;
        delay(5000);
     }
  }*/
  

  // Does something when uno says its full and the box is down 
  /*if(Serial1.available() > 0){
      incomingByte = Serial1.read();

      //data = 100 from Arduino Uno    --> Box is full
    if(incomingByte == 100){
      Serial.print("data received DEC: ");
      Serial.println(incomingByte, DEC);  // show data as DEC
      Serial.print("data received BIN: ");
      Serial.println(incomingByte, BIN);  // shows data as BIN
        
      if(boxIsAtBottom){ 
        boxIsAtBottom = false;
        relayOn();
        delay(3000);
        relayOff();
        boxIsAtTop = true;
        delay(5000);
      }
    }
  }*/
}  



// Turn off motors - Initial state
void stopMotor(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void motorTurnLeft(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  increaseSpeed();
}

void motorTurnRight(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  increaseSpeed();
}

// no instant speed
void increaseSpeed(){
  for (int i = 0; i < 124; i++) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(1);
  }
  delay(motorSpinTime);
  decreaseSpeed();
}

// no instant stop
void decreaseSpeed(){
  for (int i = 124; i >= 0; --i) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(1);
  }
  stopMotor();
}

// Methods to change relay status
void relayOn(){
   digitalWrite(Relay, RELAY_ON );
   delay(1000);
}

void relayOff(){
   digitalWrite(Relay, RELAY_OFF);
   delay(1000);
}
