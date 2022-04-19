#ifndef Music_h
#define Music_h

#ifndef NAMESIZE
#define NAMESIZE 25
#endif

class Music {
  
  private:
    int _tmpMusicCharPtr;
    char* _tmpMusicName;
    char* _musicList[NAMESIZE];    
    void resetTmpVar();

  public:
    int musicCnt;
    Music();
    void AddMusic(char musicName[NAMESIZE]);
    char* GetMusicName(int idx);
    void CollectMusicChar(char c);
};

#endif