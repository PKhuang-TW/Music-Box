#pragma execution_character_set("utf-8")

#define NAMESIZE 25

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <TMRpcm.h>

#include "MusicBox.h"

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
  while(!SD_flag){
    Serial.println("SD Failed!");
    SD_flag = SD.begin(SD_pin);
    delay(1000);
  }
  Serial.println("SD Initilaized!");
  digitalWrite(ledPin, LOW);
  
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

  tmrpcm -> quality(1);
  
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