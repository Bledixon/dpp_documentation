void setup() {
  // put your setup code here, to run once:
 pinMode(A0, INPUT);
 pinMode(2, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:


  digitalWrite(2, LOW);


  float value = analogRead(A0); 
  Serial.println(value);
  
  if(value < 100){
     Serial.println(value);
     delay(500);
  }

  digitalWrite(2, HIGH);
  value = analogRead(A0); 
  Serial.println(value);


  delay(1111);
}
