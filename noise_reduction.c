/*
//  noise_reduction.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 18/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "matutil.h"
#include "ipipe.h"

uint16_t noise_reduce(uint16_t *buffer[5], uint16_t row, uint16_t col, uint16_t threshold,int count_w,int count_col)
{

    uint16_t max,min,test;   
    /*the following variables are used to access the right buffer element */	
	static int countItr = 3 ;
	static int BMax=0 ; 
	static int BTest=2 ;
    
    /* start with the upper neighbor of same color plane */
    
	if (count_col == 1) /* when a row is completed */
	{	
	countItr = countItr + 1 ; /*Iteration count to keep track of the row*/
	}
	
	/* determine the right value of BMax and BTest	*/
	if (count_w > (5*PIXEL_ARRAY_WIDTH))
	{
		if (count_w >=((countItr*(PIXEL_ARRAY_WIDTH))+6)) 
		{		
				BMax = 0;			
				BTest = 2;			
		}
		else
		{
			BMax = 1;
			BTest = 3;
		}
	}
	
	max = min = buffer[BMax][count_col]; 	
	
	if (count_w > ((4*PIXEL_ARRAY_WIDTH)+5))       
    test = buffer[4][4];      
	else
	test = buffer[4][count_col];
	
	
	
    if (test > max)
        max = test;
    if (test < min)
        min = test;
	
    /* now process left and right neighbors, if the index permits */
    if (count_col > 1 && count_col < PIXEL_ARRAY_WIDTH-2)
    { 
        test = buffer[BMax][count_col-2];     /* upper left neighbor */		
        if (test > max)        
            max = test;
        if (test < min)
            min = test; 
        test = buffer[BMax][count_col+2];     /* upper right */
        if (test > max)
            max = test;
        if (test < min)
            min = test; 
        test = buffer[BTest][count_col-2];     /* left */ 
        if (test > max)
            test = max;
        if (test < min)
            min = test; 
        test = buffer[BTest][count_col+2];     /* right */
        if (test > max)
            max = test;
        if (test < min)
            min = test; 			
		if (count_w > ((4*PIXEL_ARRAY_WIDTH)+5))      
		test = buffer[4][2];      /* lower neighbor */
		else
		test = buffer[4][count_col-2];	/* lower left */ 	         
        if (test > max)
            max = test;
        if (test < min)
            min = test; 
        
    }
	
	
	test = buffer[BTest][count_col];       /* middle */	
		
	if (min > threshold ) 
	{      /* way below expectation */
		if (test < (min-threshold))
		{
			test = min;
		}
	}
	if (test > (max+threshold))    /* way above expectation */
		test = max;
	
	return test;
    
}

