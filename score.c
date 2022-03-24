#include <gb/gb.h>
#include "windowmap.c"
    UBYTE num, i;
    UINT16 score = 0;
    UINT16 highScore = 0;
void scoreUp(UBYTE points){
    for(i = 0; i < points; i++){
        if(score < 999){
            score += 1;
        }
        if(windowMap[8] == 0x0B){
            windowMap[8] = 0x02;
            if(windowMap[7] == 0x0B){
                windowMap[7] = 0x02;
                if(windowMap[6] == 0x0B){
                    windowMap[6] = 0x02;
                }else{
                    num = (UBYTE)(windowMap[6]);
                    num += 1;
                    windowMap[6] = (unsigned char)(num);
                }
            }else{
                num = (UBYTE)(windowMap[7]);
                num += 1;
                windowMap[7] = (unsigned char)(num);
            }
        }else{
            num = (UBYTE)(windowMap[8]);
            num += 1;
            windowMap[8] = (unsigned char)(num);
        }
        set_win_tiles(0,0,9,1,windowMap);
    }
}
void scoreDown(UBYTE points){
    for(i = 0; i < points; i++){
        if(score > 0){
            score -= 1;
        }
        if(windowMap[8] == 0x02){
            if(windowMap[7] == 0x02){
                if(windowMap[6] == 0x02){
                    return;
                }else{
                    num = (UBYTE)(windowMap[6]);
                    num -= 1;
                    windowMap[6] = (unsigned char)(num);
                    windowMap[7] = 0x0B;
                    windowMap[8] = 0x0B;
                }
            }else{
                num = (UBYTE)(windowMap[7]);
                num -= 1;
                windowMap[7] = (unsigned char)(num);
                windowMap[8] = 0x0B;
            }   
        }else{
            num = (UBYTE)(windowMap[8]);
            num -= 1;
            windowMap[8] = (unsigned char)(num);
        }
        set_win_tiles(0,0,9,1,windowMap);
    }
}
void setHighscore(){
    if(score > highScore){
        highScore = score;
        windowMap[18] = windowMap[8];
        windowMap[17] = windowMap[7];
        windowMap[16] = windowMap[6];

        windowMap[14] = 0x14;
        windowMap[13] = 0x13;
    }
    score = 0;
    windowMap[8] = 0x02;
    windowMap[7] = 0x02;
    windowMap[6] = 0x02;
}