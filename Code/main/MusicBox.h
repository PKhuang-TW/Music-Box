#ifndef MusicBox_h
#define MusicBox_h

#ifndef NAMESIZE
#define NAMESIZE 25
#endif

#include <TMRpcm.h>

class MusicBox {

    private:
        TMRpcm* _tmrpcm;

    public:
        int musicCnt;
        int musicIdx;
        int playState;

        MusicBox(TMRpcm* tmrpcm);
        void NextMusic ();
        void PrevMusic ();
        void PlayMusic (int idx);
        void StopMusic ();
};

#endif
