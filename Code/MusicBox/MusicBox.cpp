#include <MusicBox.h>
#include <string>

MusicBox::MusicBox(){
    musicIdx = 0;
    playState = 0;
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

void MusicBox::PlayMusic () {
    string musicName = to_string(idxStr);
    musicName += ".wav";
    tmrpcm.play(musicName.c_str());
    playState = 1;
}

void MusicBox::StopMusic () {
    tmrpcm.pause();
    playState = 0;
}