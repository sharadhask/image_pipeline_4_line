/*
//  lens_correction.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 16/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "matutil.h"
#include "ipipe.h"

void construct_lens_profile(uint16_t calibration, uint16_t *row_prf, uint16_t *col_prf)
{
    int i;
    short i2;
    for (i=0; i<PIXEL_ARRAY_HEIGHT; i++) {
        i2 = i-(PIXEL_ARRAY_HEIGHT>>1);
        row_prf[i] = (1<<LENS_SHADING_BITS) + ((i2*i2*calibration)>>LENS_SHADING_BITS);
    }
    for (i=0; i<PIXEL_ARRAY_WIDTH; i++) {
        i2 = i-(PIXEL_ARRAY_WIDTH>>1);
        col_prf[i] = (1<<LENS_SHADING_BITS) + ((i2*i2*calibration)>>LENS_SHADING_BITS);
    }
    
}

uint16_t lens_shading_correction(uint16_t pixel, uint16_t row_cor, uint16_t col_cor) 
{
    
    return ((pixel*row_cor*col_cor)>>(2*LENS_SHADING_BITS));
}
