#pragma execution_character_set("utf-8")

#define NAMESIZE 25

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <TMRpcm.h>

#include "Music.h"
#include "MusicBox.h"

File root;
File myFile;
TMRpcm* tmrpcm;
MusicBox myMusicBox;

const int SD_pin = 10;
const int playPin = 7;
const int nextPin = 5;
const int prevPin = 6;
const int ledPin = 8;

void setup() {

  pinMode(playPin, INPUT);
  pinMode(nextPin, INPUT);
  pinMode(prevPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
  digitalWrite(ledPin, HIGH);
  bool SD_flag = SD.begin(SD_pin);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  while(!SD_flag){
    Serial.println("SD Failed!");
    SD_flag = SD.begin(SD_pin);
    delay(1000);
  }
  Serial.println("SD Initilaized!");
  digitalWrite(ledPin, LOW);
  
  myFile = SD.open("MusicNum.txt");
  if (myFile) {
    Music myMusic;
    while (myFile.available()) {
      myMusic.CollectMusicChar(myFile.read());  // add char in order
    }
    myMusic.CollectMusicChar('\n');  // end music list
    myFile.close();  // close the file:

    tmrpcm -> quality(1);  // Audio Output
    tmrpcm -> setVolume(5);
    tmrpcm -> speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc

    // Print Music List
    Serial.println("Print Music List:");
    for(int idx=0 ; idx<myMusic.musicCnt ; idx++){
      Serial.println(myMusic.GetMusicName(idx));
    }

    MusicBox myMusicBox(tmrpcm, myMusic.musicCnt);
  }
  else {  // no file read
    Serial.println("error opening MusicNum.txt");
  }
}

boolean buttonPressed (int pin) {
  int _buttonPressed = !digitalRead(pin);
  return _buttonPressed;
}

void loop(void) {
  
  if (myFile){
    // Play
    if ( buttonPressed(playPin) ){      

      if( myMusicBox.playState ){
        Serial.println("Stop");
        myMusicBox.StopMusic();
      }
      else{
        Serial.print("Playing Music #");
        Serial.println(myMusicBox.musicIdx);
        myMusicBox.PlayMusic(myMusicBox.musicIdx);
      }
    }

    // Next
    if ( buttonPressed(nextPin) ){
        myMusicBox.NextMusic();
        Serial.print("Selecting Music #");
        Serial.println(myMusicBox.musicIdx);
      
      if( myMusicBox.playState ){
        Serial.print("Playing Music #");
        Serial.println(myMusicBox.musicIdx);
        myMusicBox.PlayMusic(myMusicBox.musicIdx);
      }
    }

    // Prev
    if ( buttonPressed(prevPin) ){
        myMusicBox.PrevMusic();
        Serial.print("Selecting Music #");
        Serial.println(myMusicBox.musicIdx);

      if( myMusicBox.playState ){
        Serial.print("Playing Music #");
        Serial.println(myMusicBox.musicIdx);
        myMusicBox.PlayMusic(myMusicBox.musicIdx);
      }
    }
  }
  else {  // led blinks if no file in SD card
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(1000);
  }
}
