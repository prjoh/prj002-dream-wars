#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


// TODO: Mix_SetPosition: https://wiki.libsdl.org/SDL_mixer/Mix_SetPosition
class Audio {
public:
    static int init(const char* sfx_path, const char* mus_path, int num_tracks = 8);
    static void clean();
    static int play_sfx(const std::string &key, int channel = -1, int loops = 0);
    static int set_volume_sfx(const std::string &key, int volume);
    static int play_long(const std::string &key, int loops = 0);
    static void set_volume_long(int volume);
    static int create_tracks_group(int from, int to, const std::string& tag);
    static int available_group_track(const std::string& tag);
    static void stop_group(const std::string& tag);
private:
    static std::unordered_map<std::string, Mix_Chunk*> sfx_clips;
    static std::unordered_map<std::string, Mix_Music*> long_clips;
    static std::unordered_map<std::string, int> tags;
    static int tag_ndx;
};
