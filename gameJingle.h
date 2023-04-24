#ifndef STARTUPJINGLE_H
#define STARTUPJINGLE_H

#include "speaker.h"

extern Speaker mySpeaker;

class GameJingle
{
    public:
    
    //move this to its own implementation file!!
    void playIntro()
    {
        //make something a little better!!!! 
        mySpeaker.PlayNote(400.0,0.1,0.2);
    }
    
};


#endif
