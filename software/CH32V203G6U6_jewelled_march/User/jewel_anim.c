/*
 * jewel_anim.c
 *
 *  Created on: 4 Mar 2025
 *      Author: matth
 */

#include "jewel_anim.h"

static AnimationType_t current_animation = DEFAULT_ANIM;
static double anim_step = 0; // from 0-1 representing a cycle of one BPM beat
static uint16_t current_bpm = ANIM_DEFAULT_BPM;
static double anim_step_size = 0;
static uint8_t led_vals[LED_COUNT];
static uint8_t custom_led_vals[LED_COUNT];
static uint8_t anim_running = false;

void Anim_Init(void) {
    current_animation = DEFAULT_ANIM;
    anim_step = 0;
    current_bpm = ANIM_DEFAULT_BPM;
    anim_step_size = ((double)current_bpm / 60.0) / ANIM_FPS;
    //printf("size %.5f\r\n", anim_step_size);
    for(uint i = 0; i < LED_COUNT; i++) {
        led_vals[i] = 0;
    }
    LEDDriver_UpdateLEDsTemp(led_vals);
    LEDDriver_Update();
}

void Anim_Start(void) {
    anim_running = true;
}

void Anim_Stop(void) {
    anim_running = false;
}

void Anim_SetAnimation(AnimationType_t anim) {
    current_animation = anim;
    anim_step = 0;
}

void Anim_CycleAnimation() {
    current_animation += 1;
    if(current_animation >= MAX_ANIM) {
        current_animation = 0;
    }
}

uint32_t loopcounter = 0;
double anim_sparkle_prevstep = 0;
// called once per frame
void Anim_UpdateFrame(void) {
    // first push currently temp frame
    LEDDriver_UpdateLEDsTemp(led_vals);
    LEDDriver_Update();

    // calc new frame for animation
    if(current_animation == ANIM_FADE) {
#define ANIM_FADE_STRENGTH 8.0 // start at -4 for fade , ramp-time (higher = faster raise/fall = more darktime)
        double ledval_raw = (-ANIM_FADE_STRENGTH*pow((anim_step-0.5),2)+1.0);
        if (ledval_raw < 0) ledval_raw = 0;
        uint8_t ledval = (255.0*ledval_raw);
        for(uint8_t i = 0; i < LED_COUNT; i++) {
            led_vals[i] = ledval;
        }
    } else if (current_animation == ANIM_WAVE) {
        for(uint8_t i = 0; i < LED_COUNT; i++) {
#define ANIM_WAVE_DAMPING 10.0
#define ANIM_WAVE_WAVECOUNT 3.0
#define ANIM_WAVE_SPEEDINCREASE 1.0
            double anim_step_offset = ANIM_WAVE_SPEEDINCREASE*anim_step+(ANIM_WAVE_WAVECOUNT/(double)LED_COUNT)*i;
            while(anim_step_offset >= 1) anim_step_offset -= 1;
            double ledval_raw = (-ANIM_WAVE_DAMPING*pow((anim_step_offset),2)+1.0);
            led_vals[i] = (255.0*ledval_raw);
        }
    } else if (current_animation == ANIM_SPARKLE) {
        for(uint8_t i = 0; i < LED_COUNT; i++) {
#define ANIM_SPARKE_PROB RAND_M/150
            if(led_vals[i] > 0) {
                int32_t tempval = ((34* (int16_t)led_vals[i]) / 35) - 1;
                if (tempval < 0) tempval = 0;
                led_vals[i] = tempval;
            }
            if(rand() < ANIM_SPARKE_PROB && led_vals[i] == 0) {
                led_vals[i] = 255;
            }
        }
    } else if (current_animation == ANIM_SPARKLE2) {
#define ANIM_SPARKE2_PROB RAND_M/2
        // anim_step hasnt looped back yet, we still have to reduce current leds
        if(anim_sparkle_prevstep < anim_step) {
            for(uint8_t i = 0; i < LED_COUNT; i++) {
                if (led_vals[i] != 0) led_vals[i] = 255.0 * (1 - anim_step);
            }
        } else { // we looped!, calc new leds to light up
            for(uint8_t i = 0; i < LED_COUNT; i++) {
                led_vals[i] = 0;
                if(rand() < ANIM_SPARKE2_PROB) led_vals[i] = 255;
            }
        }
        anim_sparkle_prevstep = anim_step;
    } else if (current_animation == ANIM_WAVE_DOWN) {
        for(uint8_t i = 0; i < LED_COUNT/2; i++) {
#define ANIM_WAVE_DOWN_DAMPING 3
#define ANIM_WAVE_DOWN_WAVECOUNT 0.75
#define ANIM_WAVE_DOWN_SPEEDINCREASE 1.0
            double anim_step_offset = ANIM_WAVE_DOWN_SPEEDINCREASE*anim_step+(ANIM_WAVE_DOWN_WAVECOUNT/((double)LED_COUNT/2.0))*i;
            while(anim_step_offset >= 1) anim_step_offset -= 1;
            double ledval_raw = (-ANIM_WAVE_DOWN_DAMPING*pow((anim_step_offset),2)+1.0);
            led_vals[i] = (255.0*ledval_raw);
            led_vals[LED_COUNT-i-1] = led_vals[i];
        }
    } else if (current_animation == ANIM_WAVE_UP) {
        for(uint8_t i = 0; i < LED_COUNT/2; i++) {
#define ANIM_WAVE_UP_DAMPING 6
#define ANIM_WAVE_UP_WAVECOUNT 1
#define ANIM_WAVE_UP_SPEEDINCREASE 1.0
            double anim_step_offset = ANIM_WAVE_DOWN_SPEEDINCREASE*anim_step+(ANIM_WAVE_UP_WAVECOUNT/((double)LED_COUNT/2.0))*i;
            while(anim_step_offset >= 1) anim_step_offset -= 1;
            double ledval_raw = (-ANIM_WAVE_UP_DAMPING*pow((anim_step_offset),2)+1.0);
            led_vals[(LED_COUNT/2)-i-1] = (255.0*ledval_raw);
            led_vals[LED_COUNT-i-1] = led_vals[i];
        }
    } else if (current_animation == ANIM_WAVE_LEFT) {
        for(uint8_t i = 0; i < (LED_COUNT/2)+1; i++) {
#define ANIM_WAVE_LEFT_DAMPING 6
#define ANIM_WAVE_LEFT_WAVECOUNT 1
#define ANIM_WAVE_LEFT_SPEEDINCREASE 1.0
            double anim_step_offset = ANIM_WAVE_LEFT_SPEEDINCREASE*anim_step+(ANIM_WAVE_LEFT_WAVECOUNT/(((double)LED_COUNT/2.0)+1))*i;
            while(anim_step_offset >= 1) anim_step_offset -= 1;
            double ledval_raw = (-ANIM_WAVE_LEFT_DAMPING*pow((anim_step_offset),2)+1.0);
            led_vals[(3*LED_COUNT/4)-i] = (255.0*ledval_raw);
            int8_t mirrorindex = (3*LED_COUNT/4)+i;
            if(mirrorindex < LED_COUNT) {
                led_vals[mirrorindex] = (255.0*ledval_raw);
            } else {
                led_vals[mirrorindex-LED_COUNT] = (255.0*ledval_raw);
            }
            //led_vals[LED_COUNT-i-1] = led_vals[i];
        }
    }  else if (current_animation == ANIM_WAVE_RIGHT) {
        for(uint8_t i = 0; i < (LED_COUNT/2)+1; i++) {
#define ANIM_WAVE_RIGHT_DAMPING 6
#define ANIM_WAVE_RIGHT_WAVECOUNT 1
#define ANIM_WAVE_RIGHT_SPEEDINCREASE 1.0
            double anim_step_offset = ANIM_WAVE_RIGHT_SPEEDINCREASE*anim_step+(ANIM_WAVE_RIGHT_WAVECOUNT/(((double)LED_COUNT/2.0)+1))*i;
            while(anim_step_offset >= 1) anim_step_offset -= 1;
            double ledval_raw = (-ANIM_WAVE_RIGHT_DAMPING*pow((anim_step_offset),2)+1.0);
            led_vals[(1*LED_COUNT/4)+i] = (255.0*ledval_raw);
            int8_t mirrorindex = (1*LED_COUNT/4)-i;
            if(mirrorindex >= LED_COUNT) {
                led_vals[mirrorindex-LED_COUNT] = (255.0*ledval_raw);
            } else if (mirrorindex < 0) {
                led_vals[mirrorindex+LED_COUNT] = (255.0*ledval_raw);
            } else {
                led_vals[mirrorindex] = (255.0*ledval_raw);
            }
            //led_vals[LED_COUNT-i-1] = led_vals[i];
        }
    }  else if (current_animation == ANIM_BEAT_FULL) {
#define ANIM_BEAT_FULL_FADE 2 //(0-1)
#define ANIM_BEAT_FULL_ONVAL 255
#define ANIM_BEAT_FULL_ONTIME 0.5
        // first scale anim-step to 0-1 depending on face and ontime
        uint32_t tempval = ANIM_BEAT_FULL_ONVAL;
        double anim_step_scaled = 0;
        if (anim_step <= ANIM_BEAT_FULL_ONTIME) {
            anim_step_scaled = 0;
            tempval = ANIM_BEAT_FULL_ONVAL;
        } else if (anim_step >= (((1-ANIM_BEAT_FULL_ONTIME)/ANIM_BEAT_FULL_FADE)+ANIM_BEAT_FULL_ONTIME) ) {
            anim_step_scaled = 1;
            tempval = 0;
        }
        else {
            anim_step_scaled = (ANIM_BEAT_FULL_FADE*(anim_step-ANIM_BEAT_FULL_ONTIME))/(1-ANIM_BEAT_FULL_ONTIME);
            //then use anim_step_scaled for any fading function from 0 to 1 (for step from 0 to 1)
            // here: (1-x) * 255 - linear
            tempval = (uint32_t)((1.0 - anim_step_scaled) * (double)ANIM_BEAT_FULL_ONVAL);
        }

        for(uint8_t i = 0; i < LED_COUNT; i++) {
            led_vals[i] = (uint8_t)tempval;
        }
    } else if (current_animation == ANIM_CUSTOM_DEBUG_UART) {
        for(uint8_t i = 0; i < LED_COUNT; i++) {
            led_vals[i] = custom_led_vals[i];
        }
    }



    // increase frame steps
    anim_step += anim_step_size;
    if(anim_step >= 1) loopcounter++;
    while(anim_step >= 1) anim_step -= 1;

    if(loopcounter > 8) {
        //Anim_CycleAnimation();
        loopcounter = 0;
    }
}

void Anim_CustomAnimationFrame(uint8_t* ledvals) {
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        custom_led_vals[i] = ledvals[i];
    }
}

void Anim_CustomAnimationLED(uint8_t led, uint8_t val) {
    custom_led_vals[led] = val;
}

AnimationType_t Anim_GetCurrentAnimation(void) {
    return current_animation;
}

void Anim_SetBeatSync(uint32_t beat_frame, uint16_t bpm);
