#include "dynos.cpp.h"
extern "C" {

//
// Options menu
//

int dynos_opt_get_value(const char *name) {
    return DynOS_Opt_GetValue(name);
}

void dynos_opt_set_value(const char *name, int value) {
    return DynOS_Opt_SetValue(name, value);
}

void dynos_opt_add_action(const char *funcname, bool (*funcptr)(const char *), bool overwrite) {
    return DynOS_Opt_AddAction(funcname, funcptr, overwrite);
}

//
// Update
//

void *dynos_update_cmd(void *cmd) {
    return DynOS_UpdateCmd(cmd);
}

void dynos_update_gfx() {
    return DynOS_UpdateGfx();
}

void dynos_update_opt(void *pad) {
    return DynOS_UpdateOpt(pad);
}

//
// Gfx
//

int dynos_gfx_import_texture(void **output, void *ptr, int tile, void *grapi, void **hashmap, void *pool, int *poolpos, int poolsize) {
    return DynOS_Gfx_ImportTexture(output, ptr, tile, grapi, hashmap, pool, (u32 *) poolpos, (u32) poolsize);
}

void dynos_gfx_swap_animations(void *ptr) {
    return DynOS_Gfx_SwapAnimations(ptr);
}

//
// Musics
//


bool dynos_music_load_presets_from_dynos_folder(const char *filename, const char *folder) {
    char filepath[256];
    char wavpath[256];
    snprintf(filepath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, filename);
    snprintf(wavpath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, folder);
    return DynOS_Music_LoadPresets(filepath, wavpath);
}

void dynos_music_play(const char *name) {
    return DynOS_Music_Play(name);
}

void dynos_music_multi_play(const char *name) {
    return DynOS_Music_Multi_Play(name);
}

void dynos_music_stop() {
    return DynOS_Music_Stop();
}

void dynos_music_pause() {
    return DynOS_Music_Pause();
}

void dynos_music_resume() {
    return DynOS_Music_Resume();
}

bool dynos_music_is_playing(const char *name) {
    return DynOS_Music_IsPlaying(name);
}

//
// Sounds
//

bool dynos_sound_load_presets_from_dynos_folder(const char *filename, const char *folder) {
    char filepath[256];
    char wavpath[256];
    snprintf(filepath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, filename);
    snprintf(wavpath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, folder);
    return DynOS_Sound_LoadPresets(filepath, wavpath);
}

void dynos_sound_play(const char *name, float *pos) {
    return DynOS_Sound_Play(name, pos);
}

void dynos_sound_stop(unsigned char bank) {
    return DynOS_Sound_Stop(bank);
}

bool dynos_sound_is_playing(const char *name) {
    return DynOS_Sound_IsPlaying(name);
}

//
// Jingles
//

bool dynos_jingle_load_presets_from_dynos_folder(const char *filename, const char *folder) {
    char filepath[256];
    char wavpath[256];
    snprintf(filepath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, filename);
    snprintf(wavpath, 256, "%s/%s/%s", DYNOS_EXE_FOLDER, DYNOS_AUDIO_FOLDER, folder);
    return DynOS_Jingle_LoadPresets(filepath, wavpath);
}

void dynos_jingle_play(const char *name) {
    return DynOS_Jingle_Play(name);
}

void dynos_jingle_stop() {
    return DynOS_Jingle_Stop();
}

void dynos_jingle_pause() {
    return DynOS_Jingle_Pause();
}

void dynos_jingle_resume() {
    return DynOS_Jingle_Resume();
}

bool dynos_jingle_is_playing(const char *name) {
    return DynOS_Jingle_IsPlaying(name);
}

}
