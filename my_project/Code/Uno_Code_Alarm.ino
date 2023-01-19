#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <FreeStack.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

// Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif

SdFat sd;
SFEMP3Shield MP3player;
int16_t last_ms_char; // milliseconds of last recieved character from Serial port.
int8_t buffer_pos; // next position to recieve character from Serial port.

char buffer[6]; // 0-35K+null

// Für den Ultraschall
  int trigger=4; //Trigger-Pin des Ultraschallsensors an Pin7 des Arduino-Boards 
  int echo=5; // Echo-Pim des Ultraschallsensors an Pin6 des Arduino-Boards 
  long dauer=0; // Das Wort dauer ist jetzt eine Variable, unter der die Zeit gespeichert wird, die eine Schallwelle bis zur Reflektion und zurück benötigt. Startwert ist hier 0.
  long entfernung=0;
  bool isInSensor = false;
  int countInterrupts = 0;
  int maxDistance = 19;



//-----------------------------------Setup ------------------------
void setup() {
  Serial.begin(115200); 
  pinMode(13, OUTPUT);        // LED on board to check if sth gets transmitted
  
  initSDCard();
  
}


void loop() {
  if(countInterrupts < 100){
    checkDistance();
    //delay(50);
  }else{
    playFullAudio();
    countInterrupts = 0;
    delay(10000);
  } 
}

//------------------------------------ Methodes

// gets the distance of the ultra sound sensor
void getDistance(){
  digitalWrite(trigger, LOW); //Hier nimmt man die Spannung für kurze Zeit vom Trigger-Pin, damit man später beim senden des Trigger-Signals ein rauschfreies Signal hat.
  delay(5); //Dauer: 5 Millisekunden
  digitalWrite(trigger, HIGH); //Jetzt sendet man eine Ultraschallwelle los.
  delay(10); //Dieser „Ton“ erklingt für 10 Millisekunden.
  digitalWrite(trigger, LOW);//Dann wird der „Ton“ abgeschaltet.
  dauer = pulseIn(echo, HIGH); //Mit dem Befehl „pulseIn“ zählt der Mikrokontroller die Zeit in Mikrosekunden, bis der Schall zum Ultraschallsensor zurückkehrt.
  entfernung = (dauer/2) * 0.03432; //Nun berechnet man die Entfernung in Zentimetern. Man teilt zunächst die Zeit durch zwei (Weil man ja nur eine Strecke berechnen möchte und nicht die Strecke hin- und zurück). Den Wert multipliziert man mit der Schallgeschwindigkeit in der Einheit Zentimeter/Mikrosekunde und erhält dann den Wert in Zentimetern.
}

// duses the distance to play audio oder count a variable
void checkDistance(){
  getDistance();

  if (entfernung >= 500 || entfernung <= 0) //Wenn die gemessene Entfernung über 500cm oder unter 0cm liegt,…
  {
    Serial.println("Kein Messwert"); //dann soll der serial monitor ausgeben „Kein Messwert“, weil Messwerte in diesen Bereichen falsch oder ungenau sind.
    countInterrupts = 0;
  }
  else //  Ansonsten…
    {
      Serial.print(entfernung); //…soll der Wert der Entfernung an den serial monitor hier ausgegeben werden.
      Serial.println(" cm"); // Hinter dem Wert der Entfernung soll auch am Serial Monitor die Einheit "cm" angegeben werden.
  
      if(entfernung < maxDistance && entfernung > 0){
        if(countInterrupts == 0){
          playAudio();
          countInterrupts++;
        }
        else{
          countInterrupts++;
        }
      }else{
        countInterrupts = 0;
      }   
    }
  
}

//set status mega
/*void setStatusMega(){
  //digitalWrite(communication, HIGH),
  //delay(3000);
  digitalWrite(communication, LOW);
}*/

//send message to mega
/*void sendMessageToMega(){
  int i = 0;
  while(i < 100){
    Serial.write(100);
    i++;
    delay(10);
  }
}*/

// plays audio
int audioShort = 1;

void playAudio(){
  Serial.println(audioShort);
  int8_t result; 
  switch (audioShort) {
    case 1:
      result = MP3player.playMP3("0001.mp3");
      audioShort++;
      break;
    case 2:
       result = MP3player.playMP3("0002.mp3");
      audioShort++;
      break;
    case 3:
      result = MP3player.playMP3("0003.mp3");
      audioShort++;
      break;
    case 4:
      result = MP3player.playMP3("0004.mp3");
      audioShort++;
      break;
    case 5:
      result = MP3player.playMP3("0005.mp3");
      audioShort++;
      break;
    case 6:
      result = MP3player.playMP3("0006.mp3");
      audioShort++;
      break;
    case 7:
      result = MP3player.playMP3("0007.mp3");
      audioShort++;
      break;
    case 8:
      result = MP3player.playMP3("0008.mp3");
      audioShort++;
      break;
    case 9:
      result = MP3player.playMP3("0009.mp3");
      audioShort = 1;
      break;
    default:
      // statements
      break;
  }
  delay(5000);
}

void playFullAudio(){
  int8_t result; 
  result = MP3player.playMP3("0020.mp3");
  delay(32000);
}

void initSDCard(){
  uint8_t result; //result code from some function as to be tested at later time.

  Serial.print(F("F_CPU = "));
  Serial.println(F_CPU);
  Serial.print(F("Free RAM = ")); // available in Version 1.0 F() bases the string to into Flash, to use less SRAM.
  Serial.print(FreeStack(), DEC);  // FreeRam() is provided by SdFatUtil.h
  Serial.println(F(" Should be a base line of 1017, on ATmega328 when using INTx"));

  pinMode(trigger, OUTPUT); // Trigger-Pin ist ein Ausgang
  pinMode(echo, INPUT); // Echo-Pin ist ein Eingang

  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();
  //check result, see readme for error codes.
  if(result != 0) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to start MP3 player"));
    if( result == 6 ) {
      Serial.println(F("Warning: patch file not found, skipping.")); // can be removed for space, if needed.
      Serial.println(F("Use the \"d\" command to verify SdCard can be read")); // can be removed for space, if needed.
    }
  }

#if (0)
  // Typically not used by most shields, hence commented out.
  Serial.println(F("Applying ADMixer patch."));
  if(MP3player.ADMixerLoad("admxster.053") == 0) {
    Serial.println(F("Setting ADMixer Volume."));
    MP3player.ADMixerVol(-3);
  }
#endif
  last_ms_char = millis(); // stroke the inter character timeout.
  buffer_pos = 0; // start the command string at zero length.

 union twobyte mp3_vol;
 //mp3_vol.word = MP3player.getVolume();
 //mp3_vol.byte[1] = 0;
 MP3player.setVolume(0,0);
}
