// got_milk.inc.c
#include "pc/configfile.h"

s32 s = 0;

static struct ObjectHitbox sMilkHitbox = {
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 80,
    /* hurtboxRadius:     */ 90,
    /* hurtboxHeight:     */ 90,
};

void bhv_milk_interact(void) 
{
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if(o->oTimer == 1) {
            r96_play_music(R96_EVENT_GOT_MILK);
            play_sound(SOUND_MENU_EXIT_PIPE, gDefaultSoundArgs);
        }
        if(o->oTimer < 50) {
            vec3f_set(gMarioState->marioObj->header.gfx.scale, (f32) o->oTimer / 8.0f, (f32) o->oTimer / 8.0f, (f32) o->oTimer / 8.0f);
            if(o->oTimer < 20)
                gMarioState->action = ACT_SHIVERING;
        }
        else {
           vec3f_set(gMarioState->marioObj->header.gfx.scale, 6.0f, 6.0f, 6.0f);
           gMarioState->milk = 1;
        }
        o->oPosY=-1000;
    }
    else {
        o->oTimer=0;
    }
    
}

void bhv_milk_init(void) 
{
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->oGravity = 4.0;
    o->oFriction = 0.91;
    obj_set_hitbox(o, &sMilkHitbox);
    cur_obj_scale(0.5f);
    if(gMarioState->numStars != 120)
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
}

void bhv_milk_loop(void) 
{
    s32 gCurrLevelArea = gCurrLevelNum * 16 + gCurrentArea->index;
    o->oFaceAngleYaw += 0x400;
    koopa_shell_spawn_sparkles(10.0f);
    bhv_milk_interact();
    if(o->oTimer > 1000) {
        bhv_milk_time_out();
    }
    set_object_visibility(o, 3000);
}

void bhv_milk_end(void) {
    vec3f_set(gMarioState->marioObj->header.gfx.scale, 1.0f, 1.0f, 1.0f);
    s = 0;
    gMarioState->milk = 0;
    gMarioState->defeatEnemy = 0;
    o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    r96_stop_music();
}

void bhv_milk_time_out(void)
{
    u16 timerValFrames;
    u16 timerMins;
    u16 timerSecs;
    u16 hide;
    timerValFrames = s;
    timerMins = timerValFrames / (30 * 60);
    timerSecs = (timerValFrames - (timerMins * 1800)) / 1;
    if (timerSecs == 50) {
        bhv_milk_end();
    } else {
        vec3f_set(gMarioState->marioObj->header.gfx.scale, 3.0 - (f32) timerSecs / 25.0, 3.0 - (f32) timerSecs / 25.0, 3.0 - (f32) timerSecs  / 25.0);
        gMarioState->action = ACT_CROUCHING;
        s++; 
    }
}

