#pragma execution_character_set("utf-8")

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <TMRpcm.h>

#include "Music.h"

//LiquidCrystal_I2C lcd(0x27, 16, 2);

File root;
File myFile;
TMRpcm tmrpcm;

const int SD_pin = 10;
const int playPin = 7;
const int nextPin = 5;
const int prevPin = 6;
const int ledPin = 8;

bool playFlag = 0;
int playState = 0;
int nextState = 0;
int prevState = 0;

int prev_playState = playState;
int prev_nextState = nextState;
int prev_prevState = prevState;

int musicIdx = 0;  // Total Music Count
int selectIdx = 0;  // Selecting Music Index
//int musicAmount;  // Total Music Count
char* musicList[NAMESIZE];
char* pBuffer;  // Declare a pointer to buffer.

String song;
char song_char[NAMESIZE] = {};

void setup() {

  pinMode(playPin, INPUT);
  pinMode(nextPin, INPUT);
  pinMode(prevPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
  digitalWrite(ledPin, HIGH);
//  lcd.begin();
  bool SD_flag = SD.begin(SD_pin);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc

  // Initial LCD 1602
//  lcd.backlight();
//  lcd.clear();
//  Serial.println("LCD Start!");


  /* 
   * Initial SD card
   */
//  lcd.setCursor(0, 0);
//  lcd.print("Initializing");
//  lcd.setCursor(0, 1);
//  lcd.print("SD Card...");
//  delay(2000);
  
  while(!SD_flag){
    Serial.println("SD Failed!");
//    lcd.setCursor(0, 0);
//    lcd.print("SD not Found!");
//    lcd.setCursor(0, 1);
//    lcd.print("Please insert SD card again.");
    SD_flag = SD.begin(SD_pin);
    delay(1000);
  }
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("SD");
//  lcd.setCursor(0, 1);
//  lcd.print("Initilaized!");
  Serial.println("SD Initilaized!");
  digitalWrite(ledPin, LOW);
  /*
   * SD Initialized
   */

  
  /*
   * Establish the musicList for LCD
   */
  myFile = SD.open("MusicNum.txt");
  if (myFile) {
    Music myMusic;
    while (myFile.available()) {
      myMusic.CollectMusicChar(myFile.read());  // add char in order
    }
    myMusic.CollectMusicChar('\n');  // end music list
    myFile.close();  // close the file:

    tmrpcm.quality(1);  // Audio Output

    // Print Music List
    Serial.println("Print Music List:");
    for(int idx=0 ; idx<myMusic.musicCnt ; idx++){
      Serial.println(myMusic.GetMusicName(idx));
    }
  }
  else {  // no file read
    Serial.println("error opening MusicNum.txt");
  }
}

void loop(void) {

  if (myFile) {
    if(selectIdx >= musicIdx)
    selectIdx = 0;

    // Play -> Pin 8
    if (playState != digitalRead(playPin)){
        
      playState = digitalRead(playPin);
      if(playState == 0)
        playFlag = !playFlag;

      if(playFlag){
        Serial.print("Playing Music #");
        Serial.println(selectIdx);
        /*
        * Play music[selectIdx]
        */
        song = String(selectIdx);
        song += ".wav";
        song.toCharArray(song_char, NAMESIZE-1);
        tmrpcm.play(song_char);
      }else{
        Serial.println("Stop");
        /*
        * Stop Music
        */
        tmrpcm.pause();
      }
    }

    // Next -> Pin 7
    if (nextState != digitalRead(nextPin)){
      nextState = digitalRead(nextPin);

      if(nextState == 1){
        selectIdx += 1;
        if(selectIdx == musicIdx)
          selectIdx = 0;
        Serial.print("Selecting Music #");
        Serial.println(selectIdx);
      }
      
      if(playFlag){
        Serial.print("Playing Music #");
        Serial.println(selectIdx);
        /*
        * Play music[selectIdx]
        */
        song = String(selectIdx);
        song += ".wav";
        song.toCharArray(song_char, NAMESIZE-1);
        tmrpcm.play(song_char);
      }
    }

    // Prev -> Pin 6
    if (prevState != digitalRead(prevPin)){
      prevState = digitalRead(prevPin);

      if(prevState == 1){
        selectIdx -= 1;
        if(selectIdx < 0)
          selectIdx = musicIdx-1;
        Serial.print("Selecting Music #");
        Serial.println(selectIdx);
      }

      if(playFlag){
        Serial.print("Playing Music #");
        Serial.println(selectIdx);
        /*
        * Play music[selectIdx]
        */
        song = String(selectIdx);
        song += ".wav";
        song.toCharArray(song_char, NAMESIZE-1);
        tmrpcm.play(song_char);
      }
    }
  }
  else {  // led blinks if no file in SD card
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(1000);
  }    
}
