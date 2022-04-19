#include "MusicBox.h"
#include <stdio.h>
#include <string.h>

MusicBox::MusicBox(TMRpcm* tmrpcm){
    musicCnt = 0;
    musicIdx = 0;
    playState = 0;
    _tmrpcm = tmrpcm;
}

void MusicBox::NextMusic () {
    musicIdx += 1;
    if(musicIdx >= musicCnt)
    musicIdx = 0;
}

void MusicBox::PrevMusic () {
    musicIdx -= 1;
    if(musicIdx < 0)
    musicIdx = musicCnt-1;
}

void MusicBox::PlayMusic (int idx) {
    char musicName[NAMESIZE];
    snprintf(musicName, NAMESIZE, "%d", idx);
    strcat (musicName, ".wav");
    _tmrpcm -> play(musicName);
    playState = 1;
}

void MusicBox::StopMusic () {
    _tmrpcm -> pause();
    playState = 0;
}
