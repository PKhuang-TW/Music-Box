#pragma execution_character_set("utf-8")

#define NAMESIZE 25

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <TMRpcm.h>

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

class Music {
  
  private:
    int _tmpMusicCharPtr;
    char* _tmpMusicName;
    char* _musicList[NAMESIZE];

    void resetTmpVar() {
      _tmpMusicCharPtr = 0;
      _tmpMusicName = malloc(NAMESIZE);
    }

  public:
    int musicCnt;

    void init() {
      musicCnt = 0;
      _tmpMusicCharPtr = 0;
      _tmpMusicName = malloc(NAMESIZE);
    }

    void AddMusic(char musicName[NAMESIZE]) {
      _musicList[musicCnt] = musicName;
      musicCnt += 1;
    }

    char* GetMusicName(int idx) {
      return _musicList[idx];
    }

    void CollectMusicChar(char c) {
      if (c == '\n') {
        _tmpMusicName[_tmpMusicCharPtr] = '\0';
        AddMusic(_tmpMusicName);
        resetTmpVar();
      }
      else {
        _tmpMusicName[_tmpMusicCharPtr] = c;
        _tmpMusicCharPtr += 1;
      }
    }
}

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
    Music myMusic();
    while (myFile.available) {
      myMusic.CollectMusicChar(myFile.read());  // add char in order
    }
    myMusic.CollectMusicChar('\n');  // end music list
    myFile.close();  // close the file:
  }
  else {  // no file read
    Serial.println("error opening MusicNum.txt");
  }

  /*
   * Audio Output
   */
  tmrpcm.quality(1);
  

  /*
   * Print Music List
   */
  Serial.println("Print Music List:");
  for(int idx=0 ; idx<=myMusic.musicCnt ; idx++){
    Serial.println(myMusic.GetMusicName[idx]);
  }
}

void loop(void) {
  
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
