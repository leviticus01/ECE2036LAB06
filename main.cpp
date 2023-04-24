//Please note that you must transform this using a 
//polymorphic screen manage like we talked about
//in the lab.  This code is meant to be 'fixed'
//and has some BAD programming choices; however,
//it does  show examples of specific mbed
//functionality that you can use in your final GOOD program. :-)

#include "mbed.h"
#include "TMP36.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "speaker.h"
#include "joystick.h"
#include "gameJingle.h"
#include "AIBot.h"

//Put this as C++ constant inside
//your class that contains the AIBot!! 
#define SPRITE_HEIGHT 8
#define SPRITE_WIDTH 11

#define _ BLACK
#define X LGREY
#define R DGREY
#define Y RED

//sometimes putting in the global namespace and
//static memory can be a little more efficient for
//embedded programming

int AIBot_sprite[SPRITE_HEIGHT * SPRITE_WIDTH] = {
    _,_,_,_,X,X,X,R,R,_,_,
    _,X,X,X,X,X,X,X,R,X,_,
    X,X,R,X,X,Y,Y,X,X,X,X,
    X,X,X,R,X,Y,Y,X,X,X,_,
    X,R,X,X,X,X,X,X,R,R,R,
    _,_,_,R,X,_,R,R,_,_,_,
    _,_,R,R,_,_,_,R,R,_,_,
    R,R,_,_,_,R,_,_,_,R,R
};


uLCD_4DGL uLCD(p9, p10, p11); // create a global lcd object
TMP36 myTMP36(p17);

DigitalIn pb1(p21); 
DigitalIn pb2(p22); 
DigitalIn pb3(p23); 

Speaker mySpeaker(p25);

Nav_Switch myNav(p16, p13, p14, p12, p15); //pin order on Sparkfun breakout

int main() {

    //Please have a startup and ending jingle.  I provided
    //a wimpy one note startup jingle
    GameJingle Jingle;
    AIBot buzz1;
    Jingle.playIntro();

    uLCD.cls();
    //You can use this following command if you want landscape setup
    //uLCD.display_control(LANDSCAPE);
    
    //This time(0) should return the number of seconds that the mbed
    //has been powered on.
    srand(time(0));
    
    //if you are not getting random numbers after each reset try debugging with
    //uLCD.printf("Time(0)=%i\n",time(0));
    
    //setup push buttons
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
    wait(0.3);  //This can make the system more stable as it sets up the pins
    
    //If you don't have this your system will be really slow.
    //This sets the communication rate between the uLCD and the 
    //mbed system.
    uLCD.baudrate(300000);
    wait(0.3);  //Always good to wait as mbed system set itself up!

    
    //let's draw a border around the screen in a festive GREEN
    //NOTE: In your program make sure the AIBot is prevented from
    //going outside the bounds set by this boarder!!  
    uLCD.cls(); //This clears the screen
    uLCD.rectangle(0,0,120,110,GREEN);
    
    //In this sample program I have one square that randomly
    //changes position after it is captured up by the AI Bot! 
    //In your program you will have various choices for 
    //your AI Bot to capture! 
    int randXPos = (rand()%(118-SPRITE_WIDTH))+2;
    int randYPos = (rand()%(108-SPRITE_HEIGHT))+2;
    
    int points = 0; //this will keep track of the number of times the 
                    // AI Bot captures the square.
    
    //In your game you will have to keep track of points in
    //a more complex scoring scenario; however, this illustrates
    //a way that you can put text anywhere on the screen! In this
    //case it is the number of times the AI Bot captues the square.
    //NOTE: The locate function coordinates are not in units of pixels
    //but in units of the size of the character printed.
    
    uLCD.text_width(1); //You can change the size of your text if you want
    uLCD.text_height(1); //using these member functions for uLCD
    uLCD.locate(1,14); //units are not pixels but character sizes
    uLCD.printf("%i",points); //This will print out the points at bottom of screen
    
    bool GETOUT = false;
    int color = WHITE;
    
    while(!GETOUT) 
    {
        //You can read about this in the lab. Allows quick printing of graphic sprites
        uLCD.BLIT(buzz1.getXPos(), buzz1.getYPos(), SPRITE_WIDTH, SPRITE_HEIGHT, AIBot_sprite);
        
        //This is what the AI Bot 'captures'. This is so boring; you must spice up the game! 
        uLCD.filled_rectangle(randXPos, randYPos,randXPos+SPRITE_WIDTH, randYPos+SPRITE_HEIGHT,color);
    
        //In order to make the sprite move I must keep track of the old
        //position before it is changed by maneveurs on the joystick
        buzz1.copyX();
        buzz1.copyY();
        
        //Here is an example on how you can use the joystick using the joystick.h
        //You might want to put in a move() function for the AIBot
        if (myNav.up()) buzz1.increaseY();
        if (myNav.down()) buzz1.decreaseY();
        if (myNav.left()) buzz1.increaseX();
        if (myNav.right()) buzz1.decreaseX();
        if (myNav.fire())  { GETOUT = true; }
        
        //You can change the color of your square with the push
        //buttons. Notice how they are used because they are in 
        //a pull up configuration
        if (!pb1) { color = WHITE; }
        if (!pb2) {color = BLUE; }
        if (!pb3) {color = GREEN; }
                     
        //Here is an example of the AI Bot 'capturing' the square       
        //IDEA: If the AI Bot sprite and the retangle sprite intersect
        //then delete the rectanglar "target" and randomly draw in another 
        //location. 
        if (buzz1.overlap(randXPos, randYPos, randXPos+SPRITE_WIDTH, randYPos+SPRITE_HEIGHT))
        //then you need to delete retangle and redraw in another place
        {
             //Note I had the target rectangle and the AI Bot sprite be the same
             //size, but you do not have to do this.
             uLCD.filled_rectangle(randXPos, randYPos,randXPos+SPRITE_WIDTH, randYPos+SPRITE_HEIGHT,BLACK);
             
             //See if you can make a better AI Bot capture sound than this!
             mySpeaker.PlayNote(450.0,0.1,0.2);
             
             //Here I pick a new location of the target rectangle
             randXPos = (rand()%(118-SPRITE_WIDTH))+2;
             randYPos = (rand()%(108-SPRITE_HEIGHT))+2;
             
             //now update the score.  Looks like a cut and paste job...
             //make make it a function!! 
             uLCD.locate(1,14);
             uLCD.text_width(1);
             uLCD.text_height(1);
             uLCD.printf("%i",++points);
        }
        
        //This will smooth out the movement of your AI Bot SPRITE  
        wait(0.1);    
        
        //If the AI Bot has been moved with the joystick then you need
        //to delete its image at the old location.  At the beginning of the loop
        //the AI Bot sprite will be redrawn again! 
        if (( buzz1.getXPos() != buzz1.getOLDXPos()) || (buzz1.getYPos() != buzz1.getOLDYPos()))       
            uLCD.filled_rectangle(buzz1.getOLDXPos(), buzz1.getOLDYPos(), 
            buzz1.getOLDXPos()+SPRITE_WIDTH, buzz1.getOLDYPos()+ SPRITE_HEIGHT, BLACK);
          
        }
    
        //Print a final farewell to indicate win or loss. Also have a nice closing jingle for
        //your game! 
        uLCD.cls();
        uLCD.printf("\n\n\n\n\n       Happy \n   Summer \n      Break!!");
    

}
