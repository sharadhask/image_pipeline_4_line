/*
//  colorcorrect.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 22/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include <stdio.h>
#include "ipipe.h"

void choosematrix(int16_t ccm[3][3], uint16_t awb_R, uint16_t awb_B)
{
    uint16_t dist,min_dist,illum_ndx;
   
    min_dist = 1<<15;
    dist = absdiff16(DAYLIGHT_B,awb_B) + 
               absdiff16(DAYLIGHT_R,awb_R);
    if (dist < min_dist) {
        illum_ndx = 0; /* daylight */
		min_dist = dist;
	}
	dist = absdiff16(FLUORESCENT_B,awb_B) +
               absdiff16(FLUORESCENT_R,awb_R);
	if (dist < min_dist) {
		illum_ndx = 1; /* flourescent */
		min_dist = dist;
	}
	dist = absdiff16(INCANDESCENT_B,awb_B) +
               absdiff16(INCANDESCENT_R,awb_R);
	if (dist < min_dist) {
		illum_ndx = 2; /* flourescent */
		min_dist = dist;
	}
	dist = absdiff16(CLOUDY_B,awb_B) +
               absdiff16(CLOUDY_R,awb_R);
	if (dist < min_dist) {
		illum_ndx = 3; /* flourescent */
		min_dist = dist;
	}
	switch (illum_ndx) {
		case 0:
        ccm[0][0] = 32; ccm[0][1] = 0; ccm[0][2] = 0;  /* these values filled in after calibration with mcbeth chart */
        ccm[1][0] = 0; ccm[1][1] = 32; ccm[1][2] = 0;
        ccm[2][0] = 0; ccm[2][1] = 0; ccm[2][2] = 32;
		break;
		case 1:
	    ccm[0][0] = 32; ccm[0][1] = 0; ccm[0][2] = 0;  /* these values filled in after calibration with mcbeth chart */
        ccm[1][0] = 0; ccm[1][1] = 32; ccm[1][2] = 0;
        ccm[2][0] = 0; ccm[2][1] = 0; ccm[2][2] = 32;
		break;
		case 2:
	    ccm[0][0] = 32; ccm[0][1] = 0; ccm[0][2] = 0;  /* these values filled in after calibration with mcbeth chart */
        ccm[1][0] = 0; ccm[1][1] = 32; ccm[1][2] = 0;
        ccm[2][0] = 0; ccm[2][1] = 0; ccm[2][2] = 32;
		break;
		case 3:
		ccm[0][0] = 32; ccm[0][1] = 0; ccm[0][2] = 0;  /* these values filled in after calibration with mcbeth chart */
        ccm[1][0] = 0; ccm[1][1] = 32; ccm[1][2] = 0;
        ccm[2][0] = 0; ccm[2][1] = 0; ccm[2][2] = 32;
		break;
    }
    /* more matrices may be added later */
   printf("Illuminant #%d chosen\n",illum_ndx);
}

void colorcorrect(int16_t ccm[3][3], uint16_t *red, uint16_t *green, uint16_t *blue)
{
    uint16_t localred,localgreen,localblue;
    int32_t sum;
    uint32_t temp;
       
	localred = *red; localgreen = *green; localblue = *blue;
	/* red coeffs are on row 0 */
	sum = ccm[0][0]*localred + ccm[0][1]*localgreen + ccm[0][2]*localblue;
	if (sum<0)
		sum = 0;
    /* clip the value to max range */
    temp = sum >> AWB_BITS;
    if (temp > MAX_PIXEL_VALUE)
        temp = MAX_PIXEL_VALUE;
	*red = temp;
	/* green coeffs are on row 1 */
	sum = ccm[1][0]*localred + ccm[1][1]*localgreen + ccm[1][2]*localblue;
	if (sum<0)
		sum = 0;
    temp = sum >> AWB_BITS;
    if (temp > MAX_PIXEL_VALUE)
        temp = MAX_PIXEL_VALUE;
	*green = temp;
	/* blue coeffs are on row 1 */
	sum = ccm[2][0]*localred + ccm[2][1]*localgreen + ccm[2][2]*localblue;
	if (sum<0)
		sum = 0;
    temp = sum >> AWB_BITS;
    if (temp > MAX_PIXEL_VALUE)
        temp = MAX_PIXEL_VALUE;
	*blue = temp;
	    
}
