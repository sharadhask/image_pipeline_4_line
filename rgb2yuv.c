/*
//  rgb2yuv.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 22/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include <stdio.h>
#include "ipipe.h"

void rgb2yuv(uint16_t red, uint16_t green, uint16_t blue, uint16_t *Y, uint16_t *U, uint16_t *V)
{
    int16_t RGB2YUVM[3][3] = {{66,129,25},{-38,-74,112},{112,-94,-18}};
    uint16_t YUVADD[3] = {16,128,128};
    int32_t sum;
    
    
    /* luminance y is in the 0-th row */
	
	sum =  RGB2YUVM[0][0]*red + RGB2YUVM[0][1]*green + RGB2YUVM[0][2]*blue;
	sum = sum + 128;
	sum = (sum>>8) + YUVADD[0];
	if (sum > 0)
		*Y = sum;
	else 
		*Y = 0;
     
    /* U is in the 1st row */
	
	sum =  RGB2YUVM[1][0]*red + RGB2YUVM[1][1]*green + RGB2YUVM[1][2]*blue;
	sum = sum + 128;
	sum = (sum>>8) + YUVADD[1];
	if (sum > 0)
		*U = sum;
	else 
		*U = 0;
   
    
    /* chrominance V is in the 2nd-th row */
	
	sum =  RGB2YUVM[2][0]*red + RGB2YUVM[2][1]*green + RGB2YUVM[2][2]*blue;
	sum = sum + 128;
	sum = (sum>>8) + YUVADD[2];
	if (sum > 0)
		*V = sum;
	else 
		*V = 0;
    
    
}
