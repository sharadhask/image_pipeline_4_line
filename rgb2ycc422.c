/*
//  rgb2ycbcr422.c
//  ipipe
// 422 means every row has only even entries for chroma, 0,2,4....
//
//  Created by Ramakrishna  Kakarala on 22/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "ipipe.h"

void rgb2ycbcr422(uint16_t ***Ic, uint16_t ***Iycc)
{
    uint16_t r,c,i,j;
    int16_t RGB2YCbCr[3][3] = {{66,129,25},{-38,-74,112},{112,-94,-18}};
    uint16_t YCCADD[3] = {16,128,128};
    int16_t rgb[3];
    int32_t sum;
    
    
    for (r=0; r<HEIGHT; r++)
        for (c=0; c<WIDTH; c++)
        { 
            rgb[0]=Ic[r][c][0]; rgb[1]=Ic[r][c][1]; rgb[2]=Ic[r][c][2];
            if (c % 2 == 1) /* full chroma */
            {
				for (i=0; i<3; i++) 
				{
					sum = 0;
					for (j=0; j<3; j++)
						sum +=  RGB2YCbCr[i][j]*rgb[j];
					sum = sum + 128;
					sum = (sum>>8) + YCCADD[i];
					if (sum > 0)
						Iycc[r][c][i] = sum;
					else 
						Iycc[r][c][i] = 0;
				}
           } else { /* luminance only */
           		
					sum = 0;
					for (j=0; j<3; j++)
						sum +=  RGB2YCbCr[0][j]*rgb[j];
					sum = sum + 128;
					sum = (sum>>8) + YCCADD[0];
					if (sum > 0)
						Iycc[r][c][0] = sum;
					else 
						Iycc[r][c][0] = 0;
					
					Iycc[r][c][1] = Iycc[r][c][2] = 0;
				}
           }
        
    
}
