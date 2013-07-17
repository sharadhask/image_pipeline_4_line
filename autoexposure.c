/*
//  autoexposure.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 28/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "ipipe.h"
#include <stdio.h>

void autoexposure(uint32_t rsum, uint32_t gsum, uint32_t bsum, uint16_t *Gain, uint16_t *Texp)
{
    uint32_t image_mean; 
    uint16_t current_exposure;
    uint16_t new_exposure;
    uint16_t Te,G;
    uint32_t band; 
    
    image_mean = (rsum+gsum+bsum) >> LOG2NPIX; 
    band = TARGET_MEAN >> 4;
    printf("image mean = %d targe = %d band=%d\n",image_mean,TARGET_MEAN,band);
    Te = *Texp; G = *Gain;
    current_exposure = Te * G;
    if (image_mean > (TARGET_MEAN + band))  /* reduce exposure */
    {
        new_exposure = (TARGET_MEAN*current_exposure)/image_mean;  
        printf("current=%d new exposure=%d\n",current_exposure,new_exposure);
        Te = new_exposure/G;        /* find new exposure; */
        if (Te < MIN_EXPOSURE)      
        {
			Te = MIN_EXPOSURE;      /* limit to min exposure */
            G = new_exposure/Te;    /* adjust gain */
        }
        
    } else {
             if (image_mean < (uint32_t)(TARGET_MEAN - band))  /* increase exposure */       
             {
                new_exposure = (TARGET_MEAN*current_exposure)/image_mean;
                Te = new_exposure/G;     /* find lower exposure */
                if (Te > MAX_EXPOSURE)
                {
				   Te = MAX_EXPOSURE;    /* limit to max exposure */
                   G = new_exposure/Te;  /* adjust gain accordingly */
                }
             }
    }
    /* new: make sure gain is between 1...16 */
	if (G > MAX_GAIN)
		G = MAX_GAIN;
	if (G < MIN_GAIN)
		G = MIN_GAIN;

    *Texp = Te;  *Gain = G;
    
}
