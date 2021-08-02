#include "data/r96/r96_c_includes.h"
#include "data/r96/system/r96_system.h"

float softenVolume = 1.0f;
float softenJingleVolume = 1.0f;

u32 onShell = 0;

const char* sR96CurrentCapMusic = NO_MUSIC;

// Infinite stairs

void r96_play_infinite_stairs_music() {
    /* Infinite stairs? */
    if (gCurrLevelNum == LEVEL_CASTLE && gCurrAreaIndex == 2 && gMarioState->numStars < 70) {
        if (gMarioState->floor != NULL && gMarioState->floor->room == 6) {
            if (gMarioState->pos[2] < 2540.0f) {
                r96_play_jingle(R96_EVENT_ENDLESS_STAIRS);
            } else {
                r96_stop_jingle();
            }
        }
    }
}

// Action Functions

void r96_play_action_sound(struct MarioState *m, s32 actionSound) {
    if (actionSound == SOUND_ACTION_TERRAIN_JUMP) {
        play_mario_action_sound(
                m, (m->flags & MARIO_METAL_CAP) ? (s32)SOUND_ACTION_METAL_JUMP
                                                : (s32)SOUND_ACTION_TERRAIN_JUMP, 1);
    } else {
        play_sound_if_no_flag(m, actionSound, MARIO_ACTION_SOUND_PLAYED);
    }
}

// Character Functions

void r96_play_character_sound_if_no_flag(struct MarioState *m, const char* R96_MARIO, const char* R96_LUIGI, const char* R96_WARIO, u32 flags) {
    if ((m->flags & flags) == 0) {
        if(isLuigi()) {
            dynos_sound_play(R96_LUIGI, m->marioObj->header.gfx.cameraToObject);
        } else if(isWario()) {
            dynos_sound_play(R96_WARIO, m->marioObj->header.gfx.cameraToObject);
        } else if(!isLuigi() && !isWario()) {
            dynos_sound_play(R96_MARIO, m->marioObj->header.gfx.cameraToObject);
        }
        m->flags |= flags;
    }
}

void r96_play_character_sound(struct MarioState *m, const char* R96_MARIO, const char* R96_LUIGI, const char* R96_WARIO) {
    if(isLuigi()) {
        dynos_sound_play(R96_LUIGI, m->marioObj->header.gfx.cameraToObject);
    } else if(isWario()) {
        dynos_sound_play(R96_WARIO, m->marioObj->header.gfx.cameraToObject);
    } else if(!isLuigi() && !isWario()) {
        dynos_sound_play(R96_MARIO, m->marioObj->header.gfx.cameraToObject);
    }
}

void r96_play_character_sound_no_arg(const char* R96_MARIO, const char* R96_LUIGI, const char* R96_WARIO) {
    if(isLuigi()) {
        dynos_sound_play(R96_LUIGI, gDefaultSoundArgs);
    } else if(isWario()) {
        dynos_sound_play(R96_WARIO, gDefaultSoundArgs);
    } else if(!isLuigi() && !isWario()) {
        dynos_sound_play(R96_MARIO, gDefaultSoundArgs);
    }
}

void r96_play_far_fall_sound(struct MarioState *m) {
    u32 action = m->action;
    if (!(action & ACT_FLAG_INVULNERABLE) && action != ACT_TWIRLING && action != ACT_FLYING
        && !(m->flags & MARIO_UNKNOWN_18)) {
        if (m->peakHeight - m->pos[1] > 1150.0f) {
            r96_play_character_sound(m, R96_MARIO_FALLING, R96_LUIGI_FALLING, R96_WARIO_FALLING);
            m->flags |= MARIO_UNKNOWN_18;
        }
    }
}

void r96_play_knockback_sound(struct MarioState *m) {
    if (m->actionArg == 0 && (m->forwardVel <= -28.0f || m->forwardVel >= 28.0f))
        r96_play_character_sound_if_no_flag(m, R96_MARIO_DOH, R96_LUIGI_DOH, R96_WARIO_DOH, MARIO_MARIO_SOUND_PLAYED);
    else
        r96_play_character_sound_if_no_flag(m, R96_MARIO_GRUNT, R96_LUIGI_GRUNT, R96_WARIO_GRUNT, MARIO_MARIO_SOUND_PLAYED);
}

void r96_play_character_charge_sound(struct MarioState *m) {
    if (!(m->flags & MARIO_MARIO_SOUND_PLAYED)) {
            if (random_u16() % 1 == 0)
                r96_play_character_sound(m, R96_MARIO_HELLO, R96_LUIGI_HELLO, R96_WARIO_HELLO);
            if (random_u16() % 2 == 0) 
                r96_play_character_sound(m, R96_MARIO_THROW, R96_LUIGI_THROW, R96_WARIO_THROW);
            if (random_u16() % 3 == 0)
                r96_play_character_sound(m, R96_MARIO_WAHA, R96_LUIGI_WAHA, R96_WARIO_WAHA);
        m->flags |= MARIO_MARIO_SOUND_PLAYED;
    }
}

void r96_play_character_jump_sound(struct MarioState *m) {
    if (!(m->flags & MARIO_MARIO_SOUND_PLAYED)) {
        if (m->action == ACT_TRIPLE_JUMP || m->action == ACT_WARIO_TRIPLE_JUMP) {
            if (random_u16() % 1 == 0)
                r96_play_character_sound(m, R96_MARIO_WAHA, R96_LUIGI_WAHA, R96_WARIO_WAHA);
            if (random_u16() % 2 == 0) 
                r96_play_character_sound(m, R96_MARIO_YIPPEE, R96_LUIGI_YIPPEE, R96_WARIO_YIPPEE);
            if (random_u16() % 3 == 0)
                r96_play_character_sound(m, R96_MARIO_YAHOO, R96_LUIGI_YAHOO, R96_WARIO_YAHOO);
        } else {
            if (random_u16() % 1 == 0)
                r96_play_character_sound(m, R96_MARIO_YAH, R96_LUIGI_YAH, R96_WARIO_YAH);
            if (random_u16() % 2 == 0)
                r96_play_character_sound(m, R96_MARIO_WAH, R96_LUIGI_WAH, R96_WARIO_WAH);
            if (random_u16() % 3 == 0)
                r96_play_character_sound(m, R96_MARIO_HOO, R96_LUIGI_HOO, R96_WARIO_HOO);
        }
        m->flags |= MARIO_MARIO_SOUND_PLAYED;
    }
}

void r96_play_character_flying_sound(struct MarioState *m) {
    if (random_u16() % 1 == 0)
        r96_play_character_sound(m, R96_MARIO_WAHA, R96_LUIGI_WAHA, R96_WARIO_WAHA);
    if (random_u16() % 2 == 0) 
        r96_play_character_sound(m, R96_MARIO_YIPPEE, R96_LUIGI_YIPPEE, R96_WARIO_YIPPEE);
    if (random_u16() % 3 == 0)
        r96_play_character_sound(m, R96_MARIO_YAHOO, R96_LUIGI_YAHOO, R96_WARIO_YAHOO);
}

// Shell Functions 

void r96_play_shell_music() {
    onShell = 1;
    dynos_music_stop();
    dynos_music_play(R96_EVENT_SHELL);
}

void r96_stop_shell_music() {
    onShell = 0;
    if (dynos_music_is_playing(R96_EVENT_SHELL))
        dynos_music_stop();
}

// Cap Functions

void r96_play_cap_music(const char* R96_CAP_MUSIC) {
    if (!Cheats.JBC) {
        if (R96_CAP_MUSIC != NO_MUSIC) {
            dynos_music_stop();
            dynos_music_play(R96_CAP_MUSIC);
        }
        sR96CurrentCapMusic = R96_CAP_MUSIC;
    }
}

void r96_music_fade_out() {
    softenVolume -= 0.10f;
    if (softenVolume <= 0.20f) {
        softenVolume = 0.20f;
    }
}

void r96_fadeout_cap_music() {
    if (sR96CurrentCapMusic != NO_MUSIC) {
        r96_music_fade_out();
    }
}

void r96_stop_cap_music() {
    if (sR96CurrentCapMusic != NO_MUSIC 
    && !dynos_music_is_playing(R96_EVENT_BOSS_INTRO) 
    && !dynos_music_is_playing(R96_EVENT_BOSS_THEME)
    ) {
        dynos_music_stop();
    }
    sR96CurrentCapMusic = NO_MUSIC;
}

void r96_cap_music_boss_fix() {
    // Scenario where boss music is done but mario has power up
    if (gMarioState->flags & MARIO_METAL_CAP) r96_play_cap_music(R96_EVENT_CAP_METAL);
    if (gMarioState->flags & MARIO_VANISH_CAP) r96_play_cap_music(R96_EVENT_CAP_VANISH);
    if (gMarioState->flags & MARIO_WING_CAP) r96_play_cap_music(R96_EVENT_CAP_WING);
}

// Jingle Functions

void r96_play_menu_jingle(const char* R96_JINGLE) {
    dynos_music_stop();
    if (!dynos_jingle_is_playing(R96_JINGLE)) {
        softenJingleVolume = 1.0f;
        dynos_jingle_stop();
        dynos_jingle_play(R96_JINGLE);
    }
}

void r96_play_jingle(const char* R96_JINGLE) {
    if (!dynos_jingle_is_playing(R96_JINGLE) && !dynos_jingle_is_playing(R96_EVENT_STAR_COLLECT)) {
        softenJingleVolume = 1.0f;
        dynos_jingle_stop();
        dynos_jingle_play(R96_JINGLE);
    }
}

void r96_play_collect_jingle(const char* R96_JINGLE) {
    if (!dynos_jingle_is_playing(R96_JINGLE)) {
        softenJingleVolume = 1.0f;
        dynos_jingle_stop();
        dynos_jingle_play(R96_JINGLE);
    }
}

void r96_stop_jingle() {
    dynos_jingle_stop();
}

void r96_jingle_fade_in() {
    if (!dynos_jingle_is_playing(R96_EVENT_PIRANHA_PLANT)) {
        if (softenJingleVolume < 1.0f) {
            softenJingleVolume += 0.10f;
        }
        if (softenJingleVolume >= 1.0f) {
            softenJingleVolume = 1.0f;
        }
    }
}

void r96_jingle_fade_out() {
    softenJingleVolume -= 0.10f;
    if (softenJingleVolume <= 0.20f) {
        softenJingleVolume = 0.20f;
    }
}

// Music Functions

void r96_play_music(const char* R96_MUSIC) {
    if (!dynos_music_is_playing(R96_MUSIC)) {
        dynos_music_stop();
        dynos_music_play(R96_MUSIC);
    }
}

void r96_play_multi_music(const char* R96_MUSIC) {
    if (!dynos_music_is_playing(R96_MUSIC)) {
        dynos_music_multi_play(R96_MUSIC);
    }
}

void r96_stop_music() {
    dynos_music_stop();
}

void r96_music_fade_in() {
    if (softenVolume < 1.0f) {
        softenVolume += 0.10f;
    }
    if (softenVolume >= 1.0f) {
        softenVolume = 1.0f;
    }
}

// When the fanfare is playing and the pause window opens
void r96_lower_music() {
    if (dynos_jingle_is_playing(R96_EVENT_STAR_FANFARE))
        softenJingleVolume = 1.0f;
    else
        softenJingleVolume = 0.20f;

    softenVolume = 0.20f;
}

void r96_level_music_update() {
    const char *music = r96_get_intended_level_music();

    if (dynos_jingle_is_playing(R96_MENU_FILE_SELECT) || dynos_jingle_is_playing(R96_EVENT_STAR_SELECT)){
        r96_jingle_fade_out();
        if (softenJingleVolume == 0.20f)
            dynos_jingle_stop();
    }

    if (!dynos_jingle_is_playing(R96_EVENT_CREDITS) 
    && !dynos_jingle_is_playing(R96_EVENT_TITLE_SCREEN) 
    && !dynos_jingle_is_playing(R96_EVENT_GAME_OVER)
    && !dynos_jingle_is_playing(R96_EVENT_PEACH_ENDING)
    && !dynos_jingle_is_playing(R96_EVENT_PEACH_MESSAGE)
    && !dynos_jingle_is_playing(R96_EVENT_INTRO)) {
        r96_play_infinite_stairs_music();
        // Keeps event music if meant to be playing
        if (music == NO_MUSIC)
            r96_stop_music();
        else if (!dynos_music_is_playing(music)
        && !dynos_music_is_playing(R96_EVENT_BOSS_INTRO)
        && !dynos_music_is_playing(R96_EVENT_BOSS_THEME)
        && !dynos_music_is_playing(R96_EVENT_CAP_METAL)
        && !dynos_music_is_playing(R96_EVENT_RACE)
        && !dynos_music_is_playing(R96_EVENT_GOT_MILK)
        && !dynos_jingle_is_playing(R96_EVENT_VICTORY)
        && !dynos_music_is_playing(R96_EVENT_CAP_VANISH)
        && !dynos_music_is_playing(R96_EVENT_CAP_WING)
        && !dynos_music_is_playing(R96_EVENT_SHELL)
        ) {
            if (gCurrLevelNum == LEVEL_JRB 
            || gCurrLevelNum == LEVEL_DDD 
            || gCurrLevelNum == LEVEL_WDW 
            || gCurrLevelNum == LEVEL_HMC
            || gCurrLevelNum == LEVEL_BBH
            || gCurrLevelNum == LEVEL_CASTLE)
                r96_play_multi_music(music);
            else
                r96_play_music(music);
        }

        // Special Jingle cases
        if (dynos_jingle_is_playing(R96_EVENT_ENDLESS_STAIRS)
            || dynos_jingle_is_playing(R96_EVENT_PIRANHA_PLANT)
            || dynos_jingle_is_playing(R96_EVENT_STAR_APPEAR) 
            || dynos_jingle_is_playing(R96_EVENT_TOAD_MESSAGE)
            || dynos_jingle_is_playing(R96_EVENT_RACE_FANFARE)
            || dynos_jingle_is_playing(R96_EVENT_SOLVE_PUZZLE)
            || dynos_jingle_is_playing(R96_EVENT_KOOPA_MESSAGE)
        ) {
            r96_jingle_fade_in();
            r96_music_fade_out();
        }
        else if (!dynos_jingle_is_playing(R96_EVENT_MERRY_GO_ROUND)
            && !dynos_jingle_is_playing(R96_EVENT_STAR_APPEAR) 
            && !dynos_jingle_is_playing(R96_EVENT_TOAD_MESSAGE)
            && !dynos_jingle_is_playing(R96_EVENT_RACE_FANFARE)
            && !dynos_jingle_is_playing(R96_EVENT_SOLVE_PUZZLE)
            && !dynos_jingle_is_playing(R96_EVENT_KOOPA_MESSAGE)
            && !dynos_jingle_is_playing(R96_EVENT_ENDLESS_STAIRS)
            && !dynos_jingle_is_playing(R96_EVENT_PIRANHA_PLANT)
        ) {
            r96_music_fade_in();
        }
        if (dynos_jingle_is_playing(R96_EVENT_STAR_COLLECT)
            || dynos_jingle_is_playing(R96_EVENT_KEY_COLLECT)
        ) {
            r96_stop_music();
        }
    }
}

const char *r96_get_intended_level_music() { 

    s32 gCurrLevelArea = gCurrLevelNum * 16 + gCurrentArea->index;

// R96 Jukebox Cheat
    if (Cheats.EnableCheats && Cheats.JBC) {
        switch (Cheats.JB) {
            case 0:
                return R96_LEVEL_BOB;
                break;
            case 1:
                return R96_LEVEL_INSIDE_CASTLE_LOBBY;
                break;
            case 2:
                return R96_LEVEL_JRB_PHASE_3;
                break;
            case 3:
                return R96_LEVEL_LLL_OUTSIDE;
                break;
            case 4:
                return R96_LEVEL_BOWSER_1;
                break;
            case 5:
                return R96_LEVEL_CCM;
                break;
            case 6:
                return R96_LEVEL_PSS;
                break;
            case 7:
                return R96_LEVEL_BBH_PHASE_1;
                break;
            case 8:
                return R96_LEVEL_HMC_PHASE_1;
                break;
            case 9:
                return R96_LEVEL_BITDW;
                break;
            case 10:
                return R96_LEVEL_BOWSER_3;
                break;
        }
    }

// Castle Grounds
    if (gCurrLevelNum == LEVEL_CASTLE_GROUNDS) {
        if (gCurrLevelArea == AREA_CASTLE_GROUNDS)
            if (dynos_jingle_is_playing(R96_EVENT_INTRO)
            || dynos_jingle_is_playing(R96_EVENT_PEACH_MESSAGE)
            || dynos_jingle_is_playing(R96_EVENT_PEACH_ENDING) 
            || dynos_jingle_is_playing(R96_EVENT_CREDITS)
        )
                r96_stop_music();
            return R96_LEVEL_CASTLE_GROUNDS;
    }

// Inside Castle
    if (gCurrLevelNum == LEVEL_CASTLE) {
        // Stops wing cap from playing inside the castle
        if (dynos_music_is_playing(R96_EVENT_CAP_WING) 
        || dynos_music_is_playing(R96_LEVEL_CASTLE_GROUNDS) 
        || dynos_music_is_playing(R96_LEVEL_CASTLE_COURTYARD))
            r96_stop_music();
        if (gCurrLevelArea == AREA_CASTLE_LOBBY)
            return R96_LEVEL_INSIDE_CASTLE_LOBBY;
        if (gCurrLevelArea == AREA_CASTLE_TIPPY)
            return R96_LEVEL_INSIDE_CASTLE_TIPPY;
        if (gCurrLevelArea == AREA_CASTLE_BASEMENT)
            return R96_LEVEL_INSIDE_CASTLE_BASEMENT;
    }

// Castle Courtyard
    if (gCurrLevelNum == LEVEL_CASTLE_COURTYARD)
        return R96_LEVEL_CASTLE_COURTYARD;

// Fourth Floor
    if (gCurrLevelNum == LEVEL_FOURTH_FLOOR) {
        if (gCurrLevelArea == AREA_CHARACTER_SWAP)
            return R96_LEVEL_FOURTH_FLOOR;
    }

// Bob Omb Battlefield
    if (gCurrLevelNum == LEVEL_BOB)
        return R96_LEVEL_BOB;

// Whomp's Fortress
    if (gCurrLevelNum == LEVEL_WF)
        return R96_LEVEL_WF;

// Cool Cool Mountain
    if (gCurrLevelNum == LEVEL_CCM) {
        if (gCurrLevelArea == AREA_CCM_OUTSIDE)
            return R96_LEVEL_CCM;
        if (gCurrLevelArea == AREA_CCM_SLIDE)
            return R96_LEVEL_CCM_SLIDE;
    }

// Jolly Roger Bay
    if (gCurrLevelNum == LEVEL_JRB) {
        if ((((s16) gMarioStates[0].pos[1]) > -3100) && (((s16) gMarioStates[0].pos[2]) <= -900))
            return R96_LEVEL_JRB_PHASE_3;
        if (gCurrLevelArea == AREA_JRB_MAIN)
            if ((((s16) gMarioStates[0].pos[1]) > 945) && (((s16) gMarioStates[0].pos[0]) <= -5260))
                return R96_LEVEL_JRB_PHASE_1;
            if ((((s16) gMarioStates[0].pos[1]) > 1000))
                return R96_LEVEL_JRB_PHASE_1;
        if (gCurrLevelArea == AREA_JRB_SHIP)
            return R96_LEVEL_JRB_SHIP;
        return R96_LEVEL_JRB_PHASE_2;
    }

// Big Boo's Haunt
    if (gCurrLevelNum == LEVEL_BBH) {
        if (gMarioCurrentRoom == BBH_OUTSIDE_ROOM || gMarioCurrentRoom == BBH_NEAR_MERRY_GO_ROUND_ROOM) 
            return R96_LEVEL_BBH_PHASE_2;
        return R96_LEVEL_BBH_PHASE_1;
    }

// Hazy Maze Cave
    if (gCurrLevelNum == LEVEL_HMC) {
        if ((((s16) gMarioStates[0].pos[0]) <= 0) && (((s16) gMarioStates[0].pos[1]) <= -203))
            return R96_LEVEL_HMC_PHASE_2;
        if ((((s16) gMarioStates[0].pos[0]) > 0) && (((s16) gMarioStates[0].pos[1]) <= -2400))
            return R96_LEVEL_HMC_PHASE_2;
        return R96_LEVEL_HMC_PHASE_1;
    }

// Lethal Lava Land
    if (gCurrLevelNum == LEVEL_LLL) {
        if (gCurrLevelArea == AREA_LLL_OUTSIDE)
            return R96_LEVEL_LLL_OUTSIDE;
        if (gCurrLevelArea == AREA_LLL_VOLCANO)
            return R96_LEVEL_LLL_VOLCANO;
    }

// Shifting Sand Land
    if (gCurrLevelNum == LEVEL_SSL) {
        if (gCurrLevelArea == AREA_SSL_OUTSIDE)
            return R96_LEVEL_SSL_OUTSIDE;
        if (gCurrLevelArea == AREA_SSL_PYRAMID) {
            if (!dynos_music_is_playing(R96_LEVEL_SSL_PYRAMID)) {
                r96_stop_cap_music();
            }
            return R96_LEVEL_SSL_PYRAMID;
        }
        if (gCurrLevelArea == AREA_SSL_EYEROK)
            return R96_LEVEL_SSL_EYEROK;
    }

// Dire Dire Docks
    if (gCurrLevelNum == LEVEL_DDD) {
        if (gCurrLevelArea == AREA_DDD_WHIRLPOOL)
            if ((((s16) gMarioStates[0].pos[0]) <= -800) || ((((s16) gMarioStates[0].pos[0]) <= 470) && (((s16) gMarioStates[0].pos[1]) > -2000)))
                return R96_LEVEL_DDD_PHASE_1;
        if (gCurrLevelArea == AREA_DDD_SUB)
            if ((((s16) gMarioStates[0].pos[1]) > 100))
                return R96_LEVEL_DDD_PHASE_3;
        return R96_LEVEL_DDD_PHASE_2;
    }

// Snowman Land
    if (gCurrLevelNum == LEVEL_SL) {
        if (gCurrLevelArea == AREA_SL_OUTSIDE)
            return R96_LEVEL_SL_OUTSIDE;
        if (gCurrLevelArea == AREA_SL_IGLOO)
            return R96_LEVEL_SL_IGLOO;
    }

// Wet Dry World 
    if (gCurrLevelNum == LEVEL_WDW) {
        if (gCurrLevelArea == AREA_WDW_MAIN)
                return R96_LEVEL_WDW_MAIN;
        if (gCurrLevelArea == AREA_WDW_TOWN) {
            if (dynos_music_is_playing(R96_EVENT_GOT_MILK))
                r96_stop_music();
            return R96_LEVEL_WDW_TOWN;
        }
    return R96_LEVEL_WDW_MAIN;
    }

// Tall Tall Mountain
    if (gCurrLevelNum == LEVEL_TTM) {
        if (gCurrLevelArea == AREA_TTM_OUTSIDE)
            return R96_LEVEL_TTM_OUTSIDE;
        if (gCurrLevelArea == AREA_TTM_SLIDE)
            return R96_LEVEL_TTM_SLIDE;
        if (gCurrLevelArea == AREA_TTM_SLIDE_2)
            return R96_LEVEL_TTM_SLIDE;
        if (gCurrLevelArea == AREA_TTM_SLIDE_3)
            return R96_LEVEL_TTM_SLIDE;
    }

// Tiny Huge Island
    if (gCurrLevelNum == LEVEL_THI) {
        if (gCurrLevelArea == AREA_THI_HUGE)
            return R96_LEVEL_THI_HUGE;
        if (gCurrLevelArea == AREA_THI_TINY)
            return R96_LEVEL_THI_TINY;
        if (gCurrLevelArea == AREA_THI_WIGGLER)
            return R96_LEVEL_THI_WIGGLER;
    }

// Tick Tock Clock
    if (gCurrLevelNum == LEVEL_TTC)
        return R96_LEVEL_TTC;

// Rainbow Ride
    if (gCurrLevelNum == LEVEL_RR)
        return R96_LEVEL_RR;

// Bowser in the Dark World
    if (gCurrLevelNum == LEVEL_BITDW)
        return R96_LEVEL_BITDW;

// Bowser in the Fire Sea
    if (gCurrLevelNum == LEVEL_BITFS)
        return R96_LEVEL_BITFS;

// Bowser in the Sky
    if (gCurrLevelNum == LEVEL_BITS) {
        r96_stop_jingle();
        return R96_LEVEL_BITS;
    }

// Secret Aquarium
    if (gCurrLevelNum == LEVEL_SA)
        return R96_LEVEL_SA;

// Wing Mario Over the Rainbow
    if (gCurrLevelNum == LEVEL_WMOTR)
        return R96_LEVEL_WMOTR;

// Peach's Secret Slide
    if (gCurrLevelNum == LEVEL_PSS)
        return R96_LEVEL_PSS;

// Cavern of the Metal Cap
    if (gCurrLevelNum == LEVEL_COTMC)
        return R96_LEVEL_COTMC;

// Tower of the Wing Cap
    if (gCurrLevelNum == LEVEL_TOTWC)
        return R96_LEVEL_TOTWC;

// Vanish Cap under the Moat
    if (gCurrLevelNum == LEVEL_VCUTM)
        return R96_LEVEL_VCUTM;

// Bowser fight 1
    if (gCurrLevelNum == LEVEL_BOWSER_1)
        return R96_LEVEL_BOWSER_1;

// Bowser fight 2
    if (gCurrLevelNum == LEVEL_BOWSER_2)
        return R96_LEVEL_BOWSER_2;

// Bowser fight 3
    if (gCurrLevelNum == LEVEL_BOWSER_3)
        return R96_LEVEL_BOWSER_3;

    return "EMPTY";
}

