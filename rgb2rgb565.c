/*
//  rgb2rgb565.c
//  ipipe
// 565 means red gets 5 bits at top, green 6, and blue 5
//
//  Created by Ramakrishna  Kakarala on 1/5/13
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "ipipe.h"
#define RMASK 0x00F8
#define GMASK 0x00FC
#define BMASK 0x00F8

void rgb2rgb565(uint16_t ***Ic, uint16_t **I565)
{
    uint16_t r,c;
    uint16_t red,green,blue,sum;
    
    for (r=0; r<HEIGHT; r++)
        for (c=0; c<WIDTH; c++)
        { 
           red = (Ic[r][c][0] & RMASK) >> 3;
           green = (Ic[r][c][1] & GMASK) >> 2;
           blue = (Ic[r][c][2] & BMASK) >> 3;
           sum = (red<<11) + (green<<5) + blue;  /* 16 bits have 565 format */
           I565[r][c] = sum;      
        }
    
}
