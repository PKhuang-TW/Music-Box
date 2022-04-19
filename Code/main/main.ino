#pragma execution_character_set("utf-8")

#define NAMESIZE 25

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <TMRpcm.h>

#include "MusicBox.h"

//LiquidCrystal_I2C lcd(0x27, 16, 2);

File root;
File myFile;
TMRpcm* tmrpcm;

const int SD_pin = 10;
const int playPin = 7;
const int nextPin = 5;
const int prevPin = 6;
const int ledPin = 8;

int musicCnt = 0;  // Total Music Count
//int musicAmount;  // Total Music Count
char* musicList[NAMESIZE];
char* pBuffer;  // Declare a pointer to buffer.

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
  
   tmrpcm -> speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc

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
    MusicBox myMusicBox(tmrpcm);
    unsigned int fileSize = myFile.size();  // Get the file size.
    pBuffer = (char*)malloc(fileSize + 1);  // Allocate memory for the file and a terminating null char.
    pBuffer[fileSize] = '\0';               // Add the terminating null char.
    myFile.read(pBuffer, fileSize);         // Read the file into the buffer.

    char* tmp = malloc(NAMESIZE);
    int count = 0;  // counter for char index in tmp
    for(int i=0 ; i<=fileSize ; i++){
      if (pBuffer[i] != '\n' && pBuffer[i] != '\0'){  // If isn't EOF or newline
        tmp[count] = pBuffer[i];
        count += 1;
      }
      else{  // If EOF or newline, copy to musicList
        tmp[count] = '\0';
        musicList[musicCnt] = tmp;
        count = 0;
        tmp = malloc(NAMESIZE);
        musicCnt += 1;
      }
    }
    free(tmp);
    free(pBuffer);
    myFile.close();  // close the file:
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening MusicNum.txt");
  }
  /*
   * musicList Done
   */


  /*
   * Audio Output
   */
   tmrpcm -> quality(1);
  

  /*
   * Print Music List
   */
  Serial.println("Print Music List:");
  for(int i=0 ; i<=musicCnt ; i++){
    Serial.println(musicList[i]);
  }
}

boolean buttonPressed (int pin) {
  int _buttonPressed = !digitalRead(pin);
  return _buttonPressed;
}

void loop(void) {
  
  MusicBox myMusicBox(tmrpcm);
  // Play
  if ( buttonPressed(playPin) ){      

    if( myMusicBox.playState ){
      Serial.println("Stop");
      myMusicBox.StopMusic();
      // tmrpcm.pause();
    }
    else{
      Serial.print("Playing Music #");
      Serial.println(myMusicBox.musicIdx);
      myMusicBox.PlayMusic(myMusicBox.musicIdx);
      // tmrpcm.play(musicName);
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
      // tmrpcm.play(musicName);
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
      // tmrpcm.play(musicName);
    }
  }
}
