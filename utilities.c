/*
//  utilities.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 22/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "ipipe.h"

uint16_t absdiff16(uint16_t x, uint16_t y)
{
    if (x < y)
        return (uint16_t)(y-x);
    else
        return (uint16_t)(x-y);
}
