//
//  update_buffer.c -- maintain circular buffer of raw pixel data
//  
//
//  Created by Ramakrishna  Kakarala on 7/5/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
//

#include "ipipe.h"
void update_buffer(uint16_t pixel, uint16_t *buffer[5])
{
    static uint16_t row = 0; 
    static uint16_t col = 0; 
	static uint16_t ind_col = 0; 
    uint16_t i;
	uint16_t *temp_1;
    
    
    if (col == PIXEL_ARRAY_WIDTH)  /* start new line */
    {
        row = (row + 1);
		col =0;
        
    }
    /* in any case, add the pixel to the buffer */
	
	if ((col == IMAGE_LINES_IN_BUFFER)&&( row == IMAGE_LINES_IN_BUFFER-1))
	{
		
		int temp;	
		
		
		if (ind_col == PIXEL_ARRAY_WIDTH)  /* over-writing first row completed  */
		{
			col= col-1;                    /* 5th row 5th element (4,4) */ 
			temp= buffer[IMAGE_LINES_IN_BUFFER-1][0]  ;    /* first element of short row */
			for (i = 0; i<IMAGE_LINES_IN_BUFFER-1; i++)
			{
				buffer[row][i]=buffer[row][i+1];            /* shift the elements of short row (one element to left)*/
			}		
			
			temp_1 = buffer[0]; 			/* shift the (2,3,4)lines */
			for (i = 0; i<IMAGE_LINES_IN_BUFFER-1; i++)
                buffer[i]=buffer[i+1];
			buffer[IMAGE_LINES_IN_BUFFER-2] = temp_1;  /* first line is now the 4th line */
			
			
			ind_col = 0 ;
			buffer[0][ind_col] = temp; // first element of short row shifted to the first row
			ind_col = ind_col+1;
			
		}
		else /*shifting the elements from short row to progressively fill the first row  */
		{
		col= col-1;
		temp= buffer[IMAGE_LINES_IN_BUFFER-1][0]  ;
		for (i = 0; i<IMAGE_LINES_IN_BUFFER-1; i++)
		{
            buffer[row][i]=buffer[row][i+1];            
		}		
		buffer[0][ind_col] = temp;		
		ind_col = ind_col+1;
		}
		
	
	}
	
    buffer[row][col] = pixel; /* */
    col = (col+1);
    

}
