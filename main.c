#include <gb/gb.h>
#include <gb/font.h>
#include <rand.h>
#include "GameChar.c"
#include "score.c"
#include "gbt_player.h"

GameChar ship, laser, rock, bigRock,subRock1,subRock2,subRock3;
BOOLEAN isShooting = FALSE,bigRockMove = TRUE;
UBYTE bigRockHit, shot;
UBYTE bigRockSplit = 0;
UBYTE direction;
font_t min_font;
extern const unsigned char * titlescreenTiles[];
extern const unsigned char * spriteTiles[];
extern const unsigned char * bkgTiles[];
extern const unsigned char * titlescreenMap[];
extern const unsigned char * bkgMusic[];
extern const unsigned char * bkgMap[];

void performanceDelay(UINT8 loopcounts){
    for(i = 0; i < loopcounts; i++){
        wait_vbl_done();
    }
}

void fadeOut(UBYTE x){
    BGP_REG = 0xE4;
    performanceDelay(x);
    BGP_REG = 0xF9;
    performanceDelay(x);
    BGP_REG = 0xFE;
    performanceDelay(x);
    BGP_REG = 0xFF;
    performanceDelay(x);
    DISPLAY_OFF;
}
void fadeIn(UBYTE x){
    DISPLAY_ON;
    BGP_REG = 0xFE;
    performanceDelay(x);
    BGP_REG = 0xF9;
    performanceDelay(x);
    BGP_REG = 0xE4;
    performanceDelay(x);
}

UINT8 randomNum(UINT8 x){
    UINT8 num;
    num = rand();
    if(num < 0) num = num * -1;
    num = num % x;
    if(num == 0){
        num = 1;
    }
    return num;
}
void moveChar(GameChar* character, UINT8 x, UINT8 y){
    move_sprite(character->spriteids[0],x,y);
    move_sprite(character->spriteids[1],x + 8 ,y);
    move_sprite(character->spriteids[2],x,y + 8);
    move_sprite(character->spriteids[3],x + 8,y + 8);
}

void moveObject(GameChar* obj, UINT8 x, UINT8 y){
    obj->x=x; obj->y=y;
    move_sprite(obj->spriteids[0],x,y);
}

UBYTE hitDetection(GameChar* one, GameChar* two){
    return (one->x >= two->x && one->x <= two->x + two->width) && (one->y >= two->y && one->y <= two->y + two->height) || (two->x >= one->x && two->x <= one->x + one->width) && (two->y >= one->y && two->y <= one->y + one->height);
}

void setupSubrocks(){
    bigRockSplit = 3;
    subRock1.x = bigRock.x - 8;
    subRock2.x = bigRock.x;
    subRock3.x = bigRock.x + 8;
    subRock1.y = bigRock.y - 4;
    subRock2.y = bigRock.y + 8;
    subRock3.y = bigRock.y - 2;
    if(bigRock.x <= 80){
        direction = 0;
    }else{
        direction = 1;
    }
    subRock1.width = 8;
    subRock2.width = 8;
    subRock3.width = 8;
    subRock1.height = 8;
    subRock2.height = 8;
    subRock2.height = 8;
    subRock1.vel = 1;
    subRock2.vel = randomNum(3);
    subRock3.vel = 1;
    subRock1.active = 1;
    subRock2.active = 1;
    subRock3.active = 1;
    bigRock.x = 200;bigRock.y=200;moveChar(&bigRock,bigRock.x,bigRock.y);

    set_sprite_tile(10,5);
    set_sprite_tile(11,5);
    set_sprite_tile(12,5);
    subRock1.spriteids[0] = 10;
    subRock2.spriteids[0] = 11;
    subRock3.spriteids[0] = 12;
    moveObject(&subRock1, subRock1.x, subRock1.y);
    moveObject(&subRock2, subRock2.x, subRock2.y);
    moveObject(&subRock3, subRock3.x, subRock3.y);
}

void setupLaser(){
    laser.x = 200;
    laser.y = 200;
    laser.width = 8;
    laser.height = 8;

    set_sprite_tile(4,4);
    laser.spriteids[0] = 4;

    moveObject(&laser, laser.x, laser.y);
}

void setupRock(){
    UINT8 num;
    num = randomNum(160);
    if(num < 20){
        num = 20;
    }
    rock.x = num;
    rock.y = 0;
    rock.width = 8;
    rock.height = 8;
    rock.vel = randomNum(3);

    set_sprite_tile(5,5);
    rock.spriteids[0] = 5;

    moveObject(&rock, rock.x, rock.y);
}
void setupship(){
    ship.x = 80;
    ship.y = 135;
    ship.width = 16;
    ship.height = 16;

    set_sprite_tile(0,0);
    ship.spriteids[0] = 0;
    set_sprite_tile(1,1);
    ship.spriteids[1] = 1;
    set_sprite_tile(2,2);
    ship.spriteids[2] = 2;
    set_sprite_tile(3,3);
    ship.spriteids[3] = 3;

    moveChar(&ship, ship.x, ship.y);
}

void setupBigRock(){
    UINT8 num;
    num = randomNum(160);
    if(num < 20){
        num = 20;
    }
    bigRock.x = num;
    bigRock.y = 0;
    bigRock.width = 16;
    bigRock.height = 16;
    bigRock.vel = 1;
    set_sprite_tile(6,6);
    bigRock.spriteids[0] = 6;
    set_sprite_tile(7,6);
    set_sprite_prop(7,S_FLIPX);
    bigRock.spriteids[1] = 7;
    set_sprite_tile(8,6);
    set_sprite_prop(8,S_FLIPX | S_FLIPY);
    bigRock.spriteids[2] = 9;
    set_sprite_tile(9,6);
    set_sprite_prop(9,S_FLIPY);
    bigRock.spriteids[3] = 8;
    moveChar(&bigRock, bigRock.x, bigRock.y);
}

void playSound(){
    NR21_REG = 0x5E;
    NR22_REG = 0x47;
    NR23_REG = 0xF4;
    NR24_REG = 0x86;
}
void playRockExplodeSound(UBYTE minus){
    NR41_REG = 0x2D;
    NR42_REG = 0xA6;
    NR43_REG = 0x64 - minus;
    NR44_REG = 0x80;
}

void laserTravel(){
    if(laser.y > 160){
        laser.x = 200;
        laser.y = 200;
        moveObject(&laser, laser.x, laser.y);
        isShooting = FALSE;
    }else if(hitDetection(&laser, &rock)){
        playRockExplodeSound(0);
        laser.x = 200;
        laser.y = 200;
        moveObject(&laser, laser.x, laser.y);
        isShooting = FALSE;
        scoreUp(2);
        setupRock();
    }else if(hitDetection(&laser, &bigRock)){
        bigRockHit += 1;
        laser.x = 200;
        laser.y = 200;
        moveObject(&laser, laser.x, laser.y);
        isShooting = FALSE;
        if(bigRockHit == 2){
            playRockExplodeSound(1);
            setupSubrocks();
            scoreUp(3);
        }
    }else if(hitDetection(&laser, &subRock1)){
        playRockExplodeSound(0);
        laser.x = 200;
        laser.y = 200;
        moveObject(&laser, laser.x, laser.y);
        subRock1.x = 200;
        subRock1.y = 200;
        moveObject(&subRock1,200,200);
        isShooting = FALSE;
        bigRockSplit -= 1;
        subRock1.active = 0;
        scoreUp(2);
    }else if(hitDetection(&laser, &subRock2)){
        playRockExplodeSound(0);
        laser.x = 200;
        laser.y = 200;
        moveObject(&laser, laser.x, laser.y);
        subRock2.x = 200;
        subRock2.y = 200;
        moveObject(&subRock2,200,200);
        isShooting = FALSE;
        bigRockSplit -= 1;
        subRock2.active = 0;
        scoreUp(2);
    }else if(hitDetection(&laser, &subRock3)){
        playRockExplodeSound(0);
        laser.x = 200;
        laser.y = 200;
        moveObject(&laser, laser.x, laser.y);
        subRock3.x = 200;
        subRock3.y = 200;
        moveObject(&subRock3,200,200);
        isShooting = FALSE;
        bigRockSplit -= 1;
        subRock3.active = 0;
        scoreUp(2);
    }else{
        laser.y -= 7;
        moveObject(&laser, laser.x, laser.y);
    }
}
void asteroidFall(){
    if(rock.y > 160){
        setupRock();
        scoreDown(1);
    }else{
        rock.y += rock.vel;
        moveObject(&rock, rock.x, rock.y);
    }
     if(bigRockMove){
         if(bigRock.y > 160){
            if(bigRockSplit == 0){
                setupBigRock();
                if(bigRockHit < 2){
                    scoreDown(8);
                }
                bigRockHit = 0;
             }
         }else{
            bigRock.y += bigRock.vel;
            moveChar(&bigRock, bigRock.x, bigRock.y);
            bigRockMove = FALSE;
         }
    }else{
        bigRockMove = TRUE;
    }
}
void subAsteroidFall(GameChar* asteroid, BOOLEAN TF){
    if(asteroid->active == 0){
        return;
    }
    if(asteroid->y > 160){
        asteroid->y = 200;
        asteroid->x = 200;
        bigRockSplit -= 1;
        asteroid->active = 0;
        scoreDown(1);
    }else if(!TF){
        asteroid->y += asteroid->vel;
    }else{
        switch (direction){
            case 0:
                asteroid->y += 1;
                asteroid->x += 1;
                break;
            case 1:
                asteroid->y += 1;
                asteroid->x -= 1;
                break;
        }
    }
    
}
void enablesound(){
	NR52_REG = 0x80;
	NR51_REG = 0xFF;
	NR50_REG = 0x77;
}
void shoot(){
    if(!isShooting && shot == 0){
        playSound();
        isShooting = TRUE;
        laser.x = ship.x + 3;
        laser.y = ship.y -2;
        moveObject(&laser, laser.x, laser.y);
        shot = 25;
    }
}

void main(){
    SWITCH_ROM_MBC1(2);
    set_bkg_data(0,190,titlescreenTiles);
    
    set_bkg_tiles(0,0,20,18,titlescreenMap);
    SWITCH_ROM_MBC1(0);
    BGP_REG = 0xFF;
    SHOW_BKG;
    
    fadeIn(15);
    waitpad(J_START);
    fadeOut(10);
    //
    HIDE_BKG;
    bigRockHit = 0;
    bigRockSplit = 0;
    score = 0;
    shot = 0;
    font_init();
    min_font = font_load(font_min);
    font_set(min_font);
    set_win_tiles(0,0,19,1,windowMap);
    move_win(7,136);
    SWITCH_ROM_MBC1(2);
    set_sprite_data(0, 7, spriteTiles);
    set_bkg_data(38,2,bkgTiles);
    set_bkg_tiles(0,0,20,18,bkgMap);
    SWITCH_ROM_MBC1(0);
    setupship();
    setupRock();
    setupLaser();
    setupBigRock();
    enablesound();
    fadeIn(10);

    disable_interrupts();

	gbt_play(bkgMusic,2,6);
	gbt_loop(1);

	set_interrupts(VBL_IFLAG);
	enable_interrupts();

    //
    SHOW_SPRITES;
    SHOW_BKG;
    SHOW_WIN;
	enable_interrupts();
    while(!hitDetection(&rock, &ship) && !hitDetection(&bigRock, &ship) && !hitDetection(&subRock1, &ship) && !hitDetection(&subRock2, &ship) && !hitDetection(&subRock3, &ship)){
            performanceDelay(1);
        if(joypad() & J_LEFT){
            if(ship.x > 8){
                ship.x -= 3;
                moveChar(&ship, ship.x, ship.y);
            }
            if(joypad() & J_A){
                shoot();
            }
        }else if(joypad() & J_RIGHT){
            if(ship.x < 154){
                ship.x += 3;
                moveChar(&ship, ship.x, ship.y);
            }
            if(joypad() & J_A){
                shoot();
            }
        }else if(joypad() & J_A){
            shoot();
        }
        if(isShooting){
            laserTravel();
        }
        if(shot > 0){
            shot -= 1;
        }
        asteroidFall();
        if(bigRockSplit > 0 && bigRockMove){
            subAsteroidFall(&subRock1, TRUE);
            moveObject(&subRock1,subRock1.x,subRock1.y);
            subAsteroidFall(&subRock2, FALSE);
            moveObject(&subRock2,subRock2.x,subRock2.y);
            subAsteroidFall(&subRock3, TRUE);
            moveObject(&subRock3,subRock3.x,subRock3.y);
        }
        gbt_update();
    }
    gbt_stop();
    enablesound();
    playRockExplodeSound(2);
    moveObject(&laser, 200,200);
    moveObject(&rock, 200,200);
    bigRock.x=200;bigRock.y=200;
    moveChar(&bigRock, 200,200);
    setupSubrocks();

    HIDE_SPRITES;
    waitpadup();
    while(!joypad()){
        performanceDelay(1);
    }
    DISPLAY_OFF;
    HIDE_WIN;
    setHighscore();
    main();
}