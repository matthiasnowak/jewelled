/*
 * jewel_config.c
 *
 *  Created on: 4 Mar 2025
 *      Author: matth
 */

#include "jewel_config.h"

volatile uint32_t system_ticks = 0;
volatile uint16_t anim_bps = 120;

static uint32_t rand_seed = 12345;

void init_rand(void) {
    rand_seed = ( *( uint32_t * )0x1FFFF7E8 ); // UID Reg 1
}

uint32_t rand(void) {
    rand_seed = (RAND_A * rand_seed + RAND_C) % RAND_M;
    return rand_seed;
}
