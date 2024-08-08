#pragma once


#define ENEMY_COUNT 10

struct AIComponent {
    float attack_range;

    float attack_cooldown = 0.0f;
    float attack_cooldown_duration;
    float attack_time = 0.0f;
    float attack_duration;

    float shoot_cooldown = 0.0f;
    float shoot_cooldown_duration;
};
