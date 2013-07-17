/*
//  black_level_subtract.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 16/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/
#include "ipipe.h"

uint16_t black_level_subtract(uint16_t pixel, uint16_t black_level)
{
    
    if (pixel < black_level)
        return 0;
    else
        return (pixel - black_level);
       
}
