/*
 * state_list.c
 *
 *  Created on: Sep 23, 2025
 *      Author: zam12
 */
#include "main.h"

uint8_t state_list[8][10] = {
		{ // Scheme 1
		    STATE_R,
		    STATE_G,
		    STATE_R,
		    STATE_G,
		    STATE_R,
		    STATE_G,
		    STATE_R,
		    STATE_G,
		    STATE_R,
		    STATE_G,
		},
		{ // Scheme 2
		    STATE_R,
		    STATE_R,
		    STATE_R,
		    STATE_Y,
		    STATE_Y,
		    STATE_Y,
		    STATE_G,
		    STATE_G,
		    STATE_G,
		    STATE_RG,
		},
		{ // Scheme 3
		    STATE_Y,
		    STATE_N,
		    STATE_Y,
		    STATE_N,
		    STATE_RG,
		    STATE_N,
		    STATE_RG,
		    STATE_N,
		    STATE_G,
		    STATE_N,
		},

		{ // Scheme 4
		    STATE_YG,
		    STATE_N,
		    STATE_N,
		    STATE_YG,
		    STATE_N,
		    STATE_N,
		    STATE_RG,
		    STATE_N,
		    STATE_N,
		    STATE_RG,
		},
		{ // Scheme 5
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		},
		{ // Scheme 6
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		},
		{ // Scheme 7
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		},
		{ // Scheme 8
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		    STATE_N,
		}

};
