#include "audio.h"


std::unordered_map<std::string, Mix_Chunk*> Audio::sfx_clips;
std::unordered_map<std::string, Mix_Music*> Audio::long_clips;
std::unordered_map<std::string, int> Audio::tags;
int Audio::tag_ndx = 0;

int Audio::init(const char* sfx_path, const char* mus_path, int num_tracks) {
  // Initialize the SDL_Mixer library with 44.1kHz stereo audio.
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_OpenAudio error: %s", Mix_GetError());
    return -1;
  }

  Mix_AllocateChannels(num_tracks);

  for (const auto & entry : std::filesystem::directory_iterator(sfx_path)) {
    std::string file_path = entry.path().string();
    int index = file_path.find_last_of("/\\");
    std::string file_name = file_path.substr(index+1);

    std::stringstream ss_file_name(file_name);
    std::string clip_name;
    getline(ss_file_name, clip_name, '.');

    std::cout << clip_name << std::endl;
    std::cout << file_path << std::endl;

    Mix_Chunk* sfx = Mix_LoadWAV(file_path.c_str());
    if (sfx == NULL) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_LoadWAV error: %s", Mix_GetError());
      return -1;
    }

    sfx_clips[clip_name] = sfx;
  }

  for (const auto & entry : std::filesystem::directory_iterator(mus_path)) {
    std::string file_path = entry.path().string();
    int index = file_path.find_last_of("/\\");
    std::string file_name = file_path.substr(index+1);

    std::stringstream ss_file_name(file_name);
    std::string clip_name;
    getline(ss_file_name, clip_name, '.');

    std::cout << clip_name << std::endl;
    std::cout << file_path << std::endl;

    Mix_Music* mus = Mix_LoadMUS(file_path.c_str());
    if (mus == NULL) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_LoadMUS error: %s", Mix_GetError());
      return -1;
    }

    long_clips[clip_name] = mus;
  }

  return 0;
}

void Audio::clean() {
  for (auto const& [_, clip] : sfx_clips)
    Mix_FreeChunk(clip);

  Mix_CloseAudio();
}


int Audio::play_sfx(const std::string &key, int channel, int loops) {
  if (sfx_clips.find(key) == sfx_clips.end()) {
    return -1;
  }

  const auto& clip = sfx_clips[key];

  int used_channel = Mix_PlayChannel(channel, clip, loops);
  if (used_channel == -1) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_PlayChannel error: %s", Mix_GetError());
    return -1;
  }

  return 0;
}

int Audio::play_long(const std::string &key, int loops) {
  if (long_clips.find(key) == long_clips.end()) {
    return -1;
  }

  const auto& clip = long_clips[key];

  int result = Mix_PlayMusic(clip, loops);
  if (result == -1) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_PlayChannel error: %s", Mix_GetError());
    return -1;
  }

  return 0;
}

int Audio::set_volume_sfx(const std::string &key, int volume) {
  if (sfx_clips.find(key) == sfx_clips.end()) {
    return -1;
  }

  const auto& clip = sfx_clips[key];

  Mix_VolumeChunk(clip, volume);

  return 0;
}

void Audio::set_volume_long(int volume) {
  Mix_VolumeMusic(volume);
}

int Audio::create_tracks_group(int from, int to, const std::string& tag) {
  if (tags.contains(tag))
    return -1;

  if (Mix_GroupChannels(from, to, tag_ndx) < 0)
    return -1;

  tags.emplace(std::make_pair(tag, tag_ndx));
  tag_ndx += 1;

  return 0;
}

int Audio::available_group_track(const std::string& tag) {
  if (!tags.contains(tag))
    return -1;

  return Mix_GroupAvailable(tags.find(tag)->second);
}

void Audio::stop_group(const std::string &tag) {
  if (!tags.contains(tag))
    return;

  Mix_HaltGroup(tags.find(tag)->second);
}

