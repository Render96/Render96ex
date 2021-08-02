#include "data/r96/r96_c_includes.h"
#include "data/r96/system/r96_system.h"

bool walk_spin = 0;
s32 spin_count = 0;
s32 charge_count = 0;

s32 check_wario_pile_driver_jump_cancel(struct MarioState *m) {
    if (m->input & INPUT_Z_PRESSED) {
        void *script;
        script = virtual_to_segmented(0x13, m->interactObj->behavior);
        if (isWario() && (script == bhvGoomba || script == bhvBreakableBoxSmall || script == bhvSmallPenguin || script == bhvMacroUkiki || script == bhvMips)) {
            return set_mario_action(m, ACT_WARIO_PILE_DRIVER, 0);
        }
    }
    return 0;
}

s32 check_wario_spin_light_idle_cancel(struct MarioState *m) {
    if (m->input & INPUT_Z_DOWN) {
        void *script;
        script = virtual_to_segmented(0x13, m->interactObj->behavior);
        if (isWario() && (script == bhvGoomba)) {
            return set_mario_action(m, ACT_PICKING_UP_ENEMIES, 0);
        }
    }
    return 0;
}

s32 check_wario_spin_heavy_idle_cancel(struct MarioState *m) {
    if (m->input & INPUT_Z_DOWN) {
        void *script;
        script = virtual_to_segmented(0x13, m->interactObj->behavior);
        if (isWario() && (script == bhvHeaveHo || script == bhvChuckya)) {
            return set_mario_action(m, ACT_PICKING_UP_ENEMIES, 0);
        }
    }
    return 0;
}

s32 act_wario_pile_driver(struct MarioState *m) {
    m->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ_SPIN;
    u32 stepResult;
    f32 yOffset;

    m->twirlYaw = m->intendedYaw;
    m->angleVel[1] = 0x2000;
    m->faceAngle[1] += m->angleVel[1];
    queue_rumble_data(4, 20);
    play_sound(SOUND_OBJ_BOWSER_SPINNING, m->marioObj->header.gfx.cameraToObject);

    if (m->actionState == 0) {
        if (m->actionTimer < 10) {
            yOffset = 20 - 2 * m->actionTimer;
             if (m->pos[1] + yOffset + 160.0f < m->ceilHeight) {
                m->pos[1] += yOffset;
                m->peakHeight = m->pos[1];
                vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
            }
        }
        mario_set_forward_vel(m, 0.0f);

        set_mario_animation(m, MARIO_ANIM_FORWARD_FLIP);
        if (m->actionTimer == 0) {
            play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
        }

        m->actionTimer++;
        if (m->actionTimer >= m->marioObj->header.gfx.curAnim.curAnim->unk08 + 4) {
            r96_play_character_sound(m, R96_MARIO_THROW, R96_LUIGI_THROW, R96_WARIO_THROW);
            m->actionState = 1;
        }
    } else {
        set_mario_animation(m, MARIO_ANIM_FORWARD_FLIP);

        stepResult = perform_air_step(m, 0);
        if (stepResult == AIR_STEP_LANDED) {
                play_mario_heavy_landing_sound(m, SOUND_ACTION_TERRAIN_HEAVY_LANDING);
                if (!check_fall_damage(m, ACT_HARD_BACKWARD_GROUND_KB)) {
                    m->particleFlags |= PARTICLE_MIST_CIRCLE | PARTICLE_HORIZONTAL_STAR;
                    set_mario_action(m, ACT_WARIO_PILE_DRIVER_LAND, 0);
                }

            set_camera_shake_from_hit(SHAKE_GROUND_POUND);
        } else if (stepResult == AIR_STEP_HIT_WALL) {
            mario_set_forward_vel(m, -16.0f);
            if (m->vel[1] > 0.0f) {
                m->vel[1] = 0.0f;
            }

            m->particleFlags |= PARTICLE_VERTICAL_STAR;
            set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
        }
    }
    return FALSE;
}

/*Coin Magnet thanks 4y*/
void coin_magnet_wario(struct MarioState *m) {
        struct Object* coinMagMove = cur_obj_nearest_object_with_behavior(bhvMovingYellowCoinWario);
        f32 oDistMove;
        if (coinMagMove != NULL && m->marioObj != NULL) {
            oDistMove = dist_between_objects(coinMagMove, m->marioObj);
        }
        while (coinMagMove != NULL && oDistMove >= 100 && oDistMove < 1000) {
            while (oDistMove >= 10) {
                coinMagMove->oPosX = approach_f32_symmetric(coinMagMove->oPosX, m->pos[0], 28);
                coinMagMove->oPosY = approach_f32_symmetric(coinMagMove->oPosY, m->pos[1], 28);
                coinMagMove->oPosZ = approach_f32_symmetric(coinMagMove->oPosZ, m->pos[2], 28);
                break;
            }
            break;
        }
        if (oDistMove == 0 && oDistMove > 1000) {
            obj_mark_for_deletion(coinMagMove);
        }
}

s32 act_wario_pile_driver_land(struct MarioState *m) {
    m->actionState = 1;
    queue_rumble_data(4, 50);
    mario_drop_held_object(m);
    if (m->input & INPUT_UNKNOWN_10) {
        return drop_and_set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BUTT_SLIDE, 0);
    }

    coin_magnet_wario(m);
    wario_landing_step(m, MARIO_ANIM_GROUND_POUND_LANDING, ACT_BUTT_SLIDE_STOP);
    return 0;
}

s32 wario_landing_step(struct MarioState *m, s32 arg1, u32 action) {
    stationary_ground_step(m);
    set_mario_animation(m, arg1);
    if (is_anim_at_end(m)) {
        r96_play_character_sound(m, R96_MARIO_SO_LONGA_BOWSER, R96_LUIGI_SO_LONGA_BOWSER, R96_WARIO_SO_LONGA_BOWSER);
        return set_mario_action(m, action, 0);
    }
    return 0;
}

s32 act_wario_charge(struct MarioState *m) {
    s16 startYaw = m->faceAngle[1];

    if (m->input & INPUT_A_PRESSED) {
        charge_count = 0;
        return set_mario_action(m, ACT_TRIPLE_JUMP, 0);
    }

    if (charge_count == 0) {
        r96_play_character_charge_sound(m);
    }
    charge_count += 1;
    if (charge_count < 60) {
        update_shell_speed(m);
        set_mario_anim_with_accel(m, MARIO_ANIM_RUNNING_UNUSED, (s32)(0xC0000));
        play_step_sound(m, 9, 45);

        switch (perform_ground_step(m)) {
            case GROUND_STEP_LEFT_GROUND:
                charge_count = 0;
                set_mario_action(m, ACT_FREEFALL, 0);
                set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
                break;

            case GROUND_STEP_HIT_WALL:
                charge_count = 0;
                mario_stop_riding_object(m);
                play_sound(m->flags & MARIO_METAL_CAP ? SOUND_ACTION_METAL_BONK : SOUND_ACTION_BONK,
                        m->marioObj->header.gfx.cameraToObject);
                m->particleFlags |= PARTICLE_VERTICAL_STAR;
                set_mario_action(m, ACT_BACKWARD_GROUND_KB, 0);
                break;

            case GROUND_STEP_NONE:
                m->flags |= MARIO_KICKING;
                m->particleFlags |= PARTICLE_DUST;
                break;
        }
        if (m->floor->type == SURFACE_BURNING) {
            charge_count = 0;
        }
        adjust_sound_for_speed(m);
        reset_rumble_timers();
    }
    else if (charge_count >= 60) {
        charge_count = 0;
        set_mario_action(m, ACT_WALKING, 0);
    }
    return FALSE;
}

s32 act_wario_triple_jump(struct MarioState *m) {
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_DIVE, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        return set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    r96_play_character_jump_sound(m);
    r96_play_action_sound(m, SOUND_ACTION_TERRAIN_JUMP);
    update_air_without_turn(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            if (m->actionState++ == 0) {
                m->vel[1] = 42.0f;
            } else {
                set_mario_action(m, ACT_FREEFALL_LAND_STOP, 0);
            }
            play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
            break;

        case AIR_STEP_HIT_WALL:
            mario_bonk_reflection(m, TRUE);
            break;
    }

    if (m->actionState == 0 || m->vel[1] > 0.0f) {
        if (set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING) == 0) {
            play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
        }
    } else {
        set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    }
    return FALSE;
}

void update_wario_spin_walk_speed(struct MarioState *m) {
    s16 intendedDYaw;
    f32 intendedMag;

    f32 face_angle;

    if (m->input & INPUT_NONZERO_ANALOG) {
        intendedDYaw = m->intendedYaw - m->faceAngle[1];
        intendedMag = m->intendedMag / 32.0f;

        m->forwardVel += coss(intendedDYaw) * intendedMag * 100.0f;
        face_angle = m->faceAngle[1] + sins(intendedDYaw) * intendedMag * 1024.0f;
        if (m->forwardVel < 0.0f) {
            face_angle += intendedDYaw;
            m->forwardVel *= -1.0f;
        }

        if (m->forwardVel > 32.0f) {
            m->forwardVel = 32.0f;
        }
    }

    m->vel[0] = m->slideVelX = m->forwardVel * sins(face_angle);
    m->vel[2] = m->slideVelZ = m->forwardVel * coss(face_angle);
}

s32 act_walking_wario_spin(struct MarioState *m) {
    update_wario_spin_walk_speed(m);
    switch (perform_ground_step(m)) {
        case GROUND_STEP_LEFT_GROUND:
            walk_spin = 0;
            spin_count = 0;
            set_mario_action(m, ACT_FREEFALL, 0);
            set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
            break;

        //case GROUND_STEP_NONE:
        //    set_mario_animation(m, MARIO_ANIM_GRAB_BOWSER);
        //    //anim_and_audio_for_walk(m);
        //    if (m->intendedMag - m->forwardVel > 16.0f) {
        //        m->particleFlags |= PARTICLE_DUST;
        //    }
        //    break;

        //case GROUND_STEP_HIT_WALL:
        //    push_or_sidle_wall(m, startPos);
        //    m->actionTimer = 0;
        //    break;
    }

    return FALSE;
}

s32 act_picking_up_enemies(struct MarioState *m) {
    if (m->actionState == 0) {
        m->actionState = 1;
        m->angleVel[1] = 0;
        m->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
        mario_grab_used_object(m);
        queue_rumble_data(5, 80);
        r96_play_character_sound(m, R96_MARIO_LIFT, R96_LUIGI_LIFT, R96_WARIO_LIFT);
    }

    m->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ_SPIN;
    set_mario_animation(m, MARIO_ANIM_GRAB_BOWSER);
    if (is_anim_at_end(m)) {
        set_mario_action(m, ACT_HOLDING_ENEMIES, 0);
    }

    stationary_ground_step(m);
    return FALSE;
}


s32 act_holding_enemies(struct MarioState *m) {
    s16 spin;

    if (m->input & INPUT_B_PRESSED) {
        if (m->angleVel[1] <= -0xE00 || m->angleVel[1] >= 0xE00) {
            r96_play_character_sound(m, R96_MARIO_SO_LONGA_BOWSER, R96_LUIGI_SO_LONGA_BOWSER, R96_WARIO_SO_LONGA_BOWSER);
        } else {
            r96_play_character_sound(m, R96_MARIO_HERE_WE_GO, R96_LUIGI_HERE_WE_GO, R96_WARIO_HERE_WE_GO);
        }
        walk_spin = 0;
        spin_count = 0;
        return set_mario_action(m, ACT_RELEASING_ENEMIES, 0);
    }

    if (m->angleVel[1] == 0) {
        if (m->actionTimer++ > 120) {
            return set_mario_action(m, ACT_RELEASING_ENEMIES, 1);
        }
        set_mario_animation(m, MARIO_ANIM_HOLDING_BOWSER);
    } else {
        m->actionTimer = 0;
        set_mario_animation(m, MARIO_ANIM_SWINGING_BOWSER);
    }

    if (m->intendedMag > 20.0f) {
        // spin = acceleration
        spin = (s16)(m->intendedYaw - m->twirlYaw) / 0x10;

        if (spin < -0x100) {
            spin = -0x100;
        }
        if (spin > 0x100) {
            spin = 0x100;
        }

        m->twirlYaw = m->intendedYaw;
        m->angleVel[1] += spin;

        if (m->angleVel[1] > 0x2000) {
            m->angleVel[1] = 0x2000;
        }
        if (m->angleVel[1] < -0x2000) {
            m->angleVel[1] = -0x2000;
        }
    } else {
        m->actionArg = 0;
        m->angleVel[1] = approach_s32(m->angleVel[1], 0, 64, 64);
    }
    if (!walk_spin) {
        if (m->angleVel[1] <= -0xE00 || m->angleVel[1] >= 0xE00) {
            walk_spin = 1;
        } else {
            stationary_ground_step(m);
        }
    }
    if (walk_spin) {
        if (m->angleVel[1] <= -0xE00) 
            m->angleVel[1] = -0x1800;
        if (m->angleVel[1] >= 0xE00)
            m->angleVel[1] = 0x1800;

        act_walking_wario_spin(m);
        spin_count += 1;

        if (spin_count == 50)
            obj_spawn_yellow_coins(m->marioObj, 1);

        if (spin_count == 60)
            obj_spawn_yellow_coins(m->marioObj, 2);

        if (spin_count == 70)
            obj_spawn_yellow_coins(m->marioObj, 1);

        coin_magnet_wario(m);
        if (spin_count >= 120) {
            r96_play_character_sound(m, R96_MARIO_SO_LONGA_BOWSER, R96_LUIGI_SO_LONGA_BOWSER, R96_WARIO_SO_LONGA_BOWSER);
            walk_spin = 0;
            spin_count = 0;
            return set_mario_action(m, ACT_RELEASING_ENEMIES, 0);
        }
    }

    // spin = starting yaw
    spin = m->faceAngle[1];
    m->faceAngle[1] += m->angleVel[1];

    // play sound on overflow
    if (m->angleVel[1] <= -0x100 && spin < m->faceAngle[1]) {
        queue_rumble_data(4, 20);
        play_sound(SOUND_OBJ_BOWSER_SPINNING, m->marioObj->header.gfx.cameraToObject);
    }
    if (m->angleVel[1] >= 0x100 && spin > m->faceAngle[1]) {
        queue_rumble_data(4, 20);
        play_sound(SOUND_OBJ_BOWSER_SPINNING, m->marioObj->header.gfx.cameraToObject);
    }
    return FALSE;
}

s32 act_releasing_enemies(struct MarioState *m) {
    if (++m->actionTimer == 1) {
        if (m->actionArg == 0) {
            queue_rumble_data(4, 50);
            mario_throw_held_object(m);
        } else {
            queue_rumble_data(4, 50);
            mario_drop_held_object(m);
        }
    }
    m->angleVel[1] = 0;
    animated_stationary_ground_step(m, MARIO_ANIM_RELEASE_BOWSER, ACT_IDLE);
    return FALSE;
}
