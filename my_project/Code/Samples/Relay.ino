
#define RELAY_ON 1          // Relay ON is on
#define RELAY_OFF 0         // Relay is of, active on O

#define Relay 52

void setup() {

  digitalWrite(Relay, RELAY_OFF);
  pinMode(Relay, OUTPUT);
  delay(1000);

}

void loop() {

   digitalWrite(Relay, RELAY_ON );
   delay(1000);

   digitalWrite(Relay, RELAY_OFF);
   delay(1000);

}
