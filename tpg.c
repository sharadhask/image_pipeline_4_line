/*
//  tpg.c  -- test pattern generator
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 15/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include <stdio.h>
#include "ipipe.h"

uint16_t test_pattern_generator(void) 
{
    uint16_t pixel;
    static unsigned int LSFR = 1u;
    
    
    LSFR = (LSFR >>1) ^ ((-(LSFR & 1u)) & 0xD0000001u);
    pixel = (LSFR & PIXELMASK); 
    
    return pixel;
}

