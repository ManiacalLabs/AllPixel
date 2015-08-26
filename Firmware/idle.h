#include "global.h"

long _lastData = 0; //Gets reset every time the AllPixel gets data
long _timeout = 30000; // run idle mode after 30 seconds

//Set every pixel to Red
inline void allRed(){
    fill_solid(_fastLEDs, numLEDs, CRGB(0,255,0));
}

//pulse 2 seconds up, 2 seconds down
inline void breath(){
    //keeping animation related vars inside as static
    static uint8_t step = 5;
    static byte dir = 1;
    static long last = 0;

    if(millis()-last > 40) //40ms * 50 = 2000ms per direction. Change by 5 each step
    {
        fill_solid(_fastLEDs, numLEDs, CRGB(0,step,0));

        step += 5*dir;
        if(step >= 250 || step <= 5)
            dir *= -1;
        last = millis();
    }
}

//whatever you want to run at boot/idle goes here.
inline void doIdle(){
    //uncomment the one you want
    // allRed();
    breath();
}
