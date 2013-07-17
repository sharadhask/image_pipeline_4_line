/*
//  auto white balance.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 18/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "ipipe.h"

uint16_t awb_sums_and_cga(uint16_t pixel, uint16_t r, uint16_t c, uint16_t awb_R, uint16_t awb_G, uint16_t awb_B, 
					  uint32_t *rsum, uint32_t *gsum, uint32_t *bsum)
{
  
   if (r % 2 == 0) /* GRGR row */
   {
		  if (c % 2 == 0) /* green */
		  {
			*gsum += pixel;
			pixel = (pixel * awb_G) >> AWB_BITS;
		  } else { 
			*rsum += pixel;
			pixel = (pixel * awb_R) >> AWB_BITS;
		  }
   } else {  /* BGBG row */
   
		if (c % 2 == 0) /* blue */
		  {
			*bsum += pixel;
			pixel = (pixel * awb_B) >> AWB_BITS;
		  } else { 
			*gsum += pixel;
			pixel = (pixel * awb_G) >> AWB_BITS;
		  }
   }
   
   return pixel;

}

void awb(uint32_t rsum,uint32_t gsum,uint32_t bsum, uint16_t *awb_R, uint16_t *awb_G, uint16_t *awb_B) 
/* awb sets color gains */
{
    uint16_t rgfactor,bgfactor;
    unsigned int adjsum;
    
  
    gsum = gsum >> 1; /* there are twice as many green as red or blue */
    
    if (gsum > LOW_AWB_THRESHOLD) { /* enough light to do white balance */
        
        rgfactor = 1<<AWB_BITS;
        adjsum = rsum;
        /* raise rsum to be near gsum */
        while (adjsum < gsum && rgfactor < (1<<(AWB_BITS+1))) 
        {
            rgfactor++;
            adjsum = (rsum*rgfactor)>>AWB_BITS;
        }
        /* or, if needed, lower rsum to be near gsum */
        while (adjsum > gsum && rgfactor > (1<<(AWB_BITS-1)))
        {
            rgfactor--;
            adjsum = (rsum*rgfactor)>>AWB_BITS;
        }
        bgfactor = 1<<AWB_BITS;
        adjsum = bsum;
        /* raise bsum to be near gsum */
        while (adjsum < gsum && bgfactor < (1<<(AWB_BITS+1))) 
        {
            bgfactor++;
            adjsum = (bsum*bgfactor)>>AWB_BITS;
        }
        /* or, if needed, lower bsum to be near gsum */
        while (adjsum > gsum && rgfactor > (1<<(AWB_BITS-1)))
        {
            bgfactor--;
            adjsum = (bsum*bgfactor)>>AWB_BITS;
        }
        *awb_B = rgfactor;
        *awb_R = bgfactor;
        
    }
    else {
       *awb_B = 1<<AWB_BITS; /* back to unit gain */
       *awb_R = 1<<AWB_BITS;
    }
    
    
}