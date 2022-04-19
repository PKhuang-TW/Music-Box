#ifndef MusicBox_h
#define MusicBox_h

#ifndef NAMESIZE
#define NAMESIZE 25
#endif

class MusicBox {

    public:
        int musicIdx;
        int playState;

        MusicBox();
        void NextMusic ();
        void PrevMusic ();
        void PlayMusic ();
        void StopMusic ();
};

#endif