/*
 * jewel_anim.h
 *
 *  Created on: 4 Mar 2025
 *      Author: matth
 */

#ifndef USER_JEWEL_ANIM_H_
#define USER_JEWEL_ANIM_H_

#include "ch32v20x.h"
#include "ch32v20x_conf.h"
#include "jewel_config.h"
#include "jewel_leddriver.h"
#include "math.h"

typedef enum {
    ANIM_FADE,      // Simple global fade
    ANIM_WAVE,      // Moving wave pattern
    ANIM_SPARKLE,   // Random sparkle effect - maybe rebrand as beat random?
    ANIM_SPARKLE2,  // or maybe this as beat random and sparke as "random" or random fade?
    ANIM_WAVE_DOWN,
    ANIM_WAVE_UP,
    ANIM_WAVE_LEFT,
    ANIM_WAVE_RIGHT,
    ANIM_WAVE_VERT_IN,
    ANIM_WAVE_VERT_OUT,
    ANIM_WAVE_HOR_IN,
    ANIM_WAVE_HOR_OUT,
    ANIM_COS_FULL,
    ANIM_SIN_COS_1,
    ANIM_SIN_COS_2,
    ANIM_BEAT_FULL, // parameters: fade (0-1), ontime (0-1) , fade maybe linear or or quadratic or sin
    ANIM_BEAT_CHECKER_1,
    ANIM_CUSTOM_DEBUG_UART,
    ANIM_BEAT_CHECKER_2,
    ANIM_BEAT_RANDOM_SIDE,
    ANIM_BEAT_RANDOM_HOR,
    ANIM_BEAT_RANDOM_VERT,
    ANIM_SPARKLE_RANDOM_SINGLE,
    ANIM_BEAT,      // Beat-synchronized pulse

} AnimationType_t;

#define DEFAULT_ANIM ANIM_WAVE_DOWN
#define MAX_ANIM 23

void Anim_Init(void);
void Anim_Start(void);
void Anim_Stop(void);
void Anim_SetAnimation(AnimationType_t anim);
void Anim_CycleAnimation();
void Anim_UpdateFrame(void);
AnimationType_t Anim_GetCurrentAnimation(void);
void Anim_SetBeatSync(uint32_t beat_frame, uint16_t bpm);
void Anim_CustomAnimationFrame(uint8_t* ledvals);
void Anim_CustomAnimationLED(uint8_t led, uint8_t val);

#endif /* USER_JEWEL_ANIM_H_ */
