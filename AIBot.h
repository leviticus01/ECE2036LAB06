#ifndef AIBOT_H
#define AIBOT_H

//I am okay with keeping these
//as define statements for the colors
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define LGREY 0xBFBFBF
#define DGREY 0x5F5F5F
#define YELLOW 0xFFFF00
#define BROWN 0x964B00

//Maybe these should be c++ constants
//defined in your GobbleBuzz class!!
#define X_DELTA 5
#define Y_DELTA 5

class AIBot
{
    public:
        AIBot(): xpos(50), ypos(50) { copyX();copyY();}
        inline void copyX(){ xOLDpos = xpos;}
        inline void copyY() {yOLDpos = ypos;}
        inline void increaseX() { xpos+=X_DELTA ;}
        inline void increaseY() { ypos+=Y_DELTA;}
        inline void decreaseX() { xpos-=X_DELTA; }
        inline void decreaseY() { ypos-=Y_DELTA; }   
        
        bool overlap(int,int,int,int);
        
        int getXPos() const { return xpos; }
        int getYPos() const { return ypos; }
        int getOLDXPos() const { return xOLDpos; }
        int getOLDYPos() const {return yOLDpos; }
         
    private:
        int xpos;
        int ypos; 
        int xOLDpos;
        int yOLDpos;   
    
};

#endif