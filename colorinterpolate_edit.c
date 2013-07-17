/*
//  colorinterpolate.c
Bayer interpolation + noise reduction in one pass.  Assumes pattern.  
GR
BG

//
//  Created by Ramakrishna  Kakarala on 15/5/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "ipipe.h"

/* sharpening kernel works by adding the output of filter
-1           -1 0 -1
-1 4 -1   or    0 4  0
-1           -1 0 -1
to the image's green plane */
static uint16_t sharpening(int16_t interp, int16_t lpass)
{
	int16_t sdiff;
	sdiff = (4*interp - lpass)>>SHARPENING_FACTOR;
	interp = interp + sdiff;
	if (interp < 0)
		interp = 0;
	if (interp > MAX_PIXEL_VALUE)
		interp = MAX_PIXEL_VALUE;
	return (uint16_t) interp;
}

void colorinterpolate_noisereduce(uint16_t **buffer, uint16_t r, uint16_t c, uint16_t noise_threshold,
								  uint16_t *red, uint16_t *green, uint16_t *blue)
{
	uint16_t hdiff,vdiff;
	int16_t interp,lpass;

	/* this model helps visualize processing.  Either we have for r = 0, 2, ..
	col  0 1 2 3 4 5 6 7 8 9 
	G R G R G R G R G R  row 0
	B G B G B G B G B G  row 1
	G R G R G R G R G R  row 2
	B G B G B G B G B G  row 3
	G R G R G R G R G R	row 4   
	or                                       for r = 1, 3, 5, 7. ....
	B G B G B G B G B G  row 0
	G R G R G R G R G R  row 1
	B G B G B G B G B G  row 2
	G R G R G R G R G R  row 3 
	B G B G B G B G B G  row 4

	we process only the middle row 
	*/

	if (c == 0 || c == (PIXEL_ARRAY_WIDTH-1))  /* edge condition, simply ignore and grey out */
	{

		*red = *green = *blue = buffer[2][c]; 

	} else {  /* proceed normally */

		if ( r % 2 == 0) /* even row GRGR.... */
		{
			if (c % 2 == 0) /* even col -- have green pixel, calculate red and blue */
			{
				interp = noise_reduce(buffer,2,c,noise_threshold);
				lpass = (buffer[1][c-1]+buffer[1][c+1]+buffer[3][c-1]+buffer[3][c+1]);
				*green = sharpening(interp,lpass);
				*red = (buffer[2][c-1] + buffer[2][c+1])>>1;  
				*blue = (buffer[1][c] + buffer[3][c])>>1;

			} else {      /* odd col -- red pixel, calculate green and blue */

				*red = noise_reduce(buffer,2,c,noise_threshold);

				/* choose green in the direction of least difference */
				hdiff = absdiff16(buffer[2][c-1],buffer[2][c+1]);
				vdiff = absdiff16(buffer[1][c],buffer[3][c]);
				lpass = (buffer[2][c-1]+buffer[2][c+1]+buffer[1][c]+buffer[3][c]);
				if (hdiff<vdiff)
				{
					interp = (buffer[2][c-1] + buffer[2][c+1])>>1;
					*green = sharpening(interp,lpass);
				} else {
					interp = (buffer[1][c] + buffer[3][c])>>1;
					*green = sharpening(interp,lpass);
				}
				/* blue is the average of the 4 neighbors */
				*blue = (buffer[1][c-1] + buffer[1][c+1] + buffer[3][c-1]+buffer[3][c+1])>>2;
			}
		} else {  /* odd row BGBGBG.... */

			if (c % 2 == 0) /* even col -- blue pixel, calculate green and red */
			{
				*blue = noise_reduce(buffer,2,c,noise_threshold); 

				/* choose green in the direction of least difference */
				hdiff = absdiff16(buffer[2][c-1],buffer[2][c+1]);
				vdiff = absdiff16(buffer[1][c],buffer[3][c]);
				lpass = (buffer[2][c-1]+buffer[2][c+1]+buffer[1][c]+buffer[3][c]);

				if (hdiff<vdiff)
				{
					interp = (buffer[2][c-1] + buffer[2][c+1])>>1;
					*green = sharpening(interp,lpass);
				} else {
					interp = (buffer[1][c] + buffer[3][c])>>1;
					*green = sharpening(interp,lpass);
				}

				/* red is the average of the 4 neighbors */
				*red = (buffer[1][c-1] + buffer[1][c+1] + buffer[3][c-1]+buffer[3][c+1])>>2;  

			} else { /* odd col, green pixel, find blue and red */
				interp = noise_reduce(buffer,2,c,noise_threshold);
				lpass = (buffer[1][c-1]+buffer[1][c+1]+buffer[3][c-1]+buffer[3][c+1]);
				*green = sharpening(interp,lpass);
				*blue = (buffer[2][c-1]+buffer[2][c+1])>>1;
				*red = (buffer[1][c] + buffer[3][c])>>1;
			} /* odd col */		

		} /* odd row BG... */	

	} /* normal processing (columns are not at the edges) */


}
