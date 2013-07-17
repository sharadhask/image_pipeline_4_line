/*
//  noise_reduction.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 18/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "matutil.h"
#include "ipipe.h"

uint16_t noise_reduce(uint16_t **buffer, uint16_t row, uint16_t col, uint16_t threshold)
{
    uint16_t max,min,test;
    
    /* start with the upper neighbor of same color plane */
    max = min = buffer[0][col]; 
    test = buffer[4][col];      /* lower neighbor */
    if (test > max)
        max = test;
    if (test < min)
        min = test;
    
    /* now process left and right neighbors, if the index permits */
    if (col > 1 && col < PIXEL_ARRAY_WIDTH-2)
    { 
        test = buffer[0][col-2];     /* upper left neighbor */
        if (test > max)        
            max = test;
        if (test < min)
            min = test; 
        test = buffer[0][col+2];     /* upper right */
        if (test > max)
            max = test;
        if (test < min)
            min = test; 
        test = buffer[2][col-2];     /* left */ 
        if (test > max)
            test = max;
        if (test < min)
            min = test; 
        test = buffer[2][col+2];     /* right */
        if (test > max)
            max = test;
        if (test < min)
            min = test; 
        test = buffer[4][col-2];     /* lower left */
        if (test > max)
            max = test;
        if (test < min)
            min = test; 
        /* the lower right is deliberately commented out
         because it is not updated to a fresh image value
         being used, i.e., it is invalid */
      /*   test = buffer[4][col+2];  */   /* lower right */
      /*   if (test > max)
             max = test;
           if (test < min)
             min = test;      */ 
    }
	
    test = buffer[2][col];       /* middle */
    
	if (min > threshold ) {      /* way below expectation */
		if (test < min-threshold) {
			test = min;
		}
	}
	if (test > max+threshold)    /* way above expectation */
		test = max;
	return test;
    
}

