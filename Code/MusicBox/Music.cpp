#include <stdlib.h>

Music::Music() {
    musicCnt = 0;
    resetTmpVar();
}

void Music::resetTmpVar() {
    _tmpMusicCharPtr = 0;
    _tmpMusicName = malloc(NAMESIZE);
}

void Music::AddMusic(char musicName[NAMESIZE]) {
    _musicList[musicCnt] = musicName;
    musicCnt += 1;
}

char* Music::GetMusicName(int idx) {
    return _musicList[idx];
}

void Music::CollectMusicChar(char c) {
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
