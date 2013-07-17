/*
 //  main.c
 //  ipipe
 //
 //  Created by Ramakrishna  Kakarala on 15/4/13.
 //  Copyright 2013 Nanyang Technological University. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include "matutil.h"
#include "ipipe.h"

void print_config(void) 
{
    printf("In this computer, sizeof int = %u\n",sizeof(int));
    printf("and sizeof uint16_t = %u\n\n",sizeof(uint16_t));
}


int main ()
{
    /* variables for sumulation only */
    uint16_t **I,**Irgb565,***Ic,***Iyuv,***Iycc, ***Ibtu;
    /* the above are matrices allocated for simulation. For a N x M image,
     with N rows and M columns, the asic will require only 5 rows by M
     colums of memory.  */
    PGMData *inputpgm, *outputpgm;   
    
    /* variables that are part of asic */
    // uint16_t **buffer; //comment this bcoz the new buffer is declared and initialized below
    uint16_t noise_threshold;
    uint32_t rsum,gsum,bsum;
    uint16_t awb_R,awb_G,awb_B;
    uint16_t Gain,Texp;
    uint16_t *row_prf, *col_prf;
   
    uint16_t r,c;            /* row, column indices */
    uint16_t r2,c2;
    uint16_t pixel;  
    uint16_t red,green,blue; /* pixel values */
    uint16_t y,u,v,prevy;    /* YUV coordinates */ 
    int16_t ccm[3][3];       /* color correction matrix */
    uint32_t edginess;       
    uint32_t edginess_window; 
    uint32_t prev_edginess;
    uint32_t focus;
    uint16_t focus_stepsize;
    int16_t focus_direction;
    uint16_t field = 0;
    InputFormat input = PIXELARRAY;
    OutputFormat output = RGB444;
    
    /*********allocate memory for simulation purposes only***************/
    
    Iyuv = ui16colorim(0,HEIGHT-1,0,WIDTH-1);
    Ic = ui16colorim(0,HEIGHT-1,0,WIDTH-1);
    
    printf("Image pipeline version %f\n",VERSION);
    print_config();
    
    inputpgm = (PGMData *)malloc(sizeof(PGMData));
    
    /*************** read input image *********************************/
    if (input != TESTPATTERN) 
    {
        readPGM("inputGRBG10bitg2p2.pgm",inputpgm);  /* gamma of this is 1/2.2, and hence needs correction */
        I = inputpgm->matrix;
    }
    
    /* code changed here!!! */
    /* OLD bUFFER:allocate buffer. In Asic, assume 5 lines of image are available */
    // buffer = ui16matrix(0,IMAGE_LINES_IN_BUFFER-1,0,PIXEL_ARRAY_WIDTH-1);
	/* New BUFFER:This a modified buffer with a different allocation: 4line implementation(with a short 5th row)*/
	uint16_t row1[PIXEL_ARRAY_WIDTH],row2[PIXEL_ARRAY_WIDTH],row3[PIXEL_ARRAY_WIDTH],row4[PIXEL_ARRAY_WIDTH],row5[5] ;
	uint16_t *buffer[5] = {row1,row2,row3,row4,row5};
    
    /* build shading correction look up tables -- can be in ROM for the ASIC */
    row_prf = ui16vector(0,inputpgm->height-1);
    col_prf = ui16vector(0,inputpgm->width-1);
    construct_lens_profile(PROFILE_CALIBRATION, row_prf, col_prf); 


    /*********here ASIC starts*********************
     *
     * input image is in matrix I 
     * Assume I represents the pixel array
     * I does not need to be stored in the ASIC
     * assume I is read one pixel at a time, in a 
     * raster scan manner 
     ***********************************************/
    
      
    /* to be done at reset */
    awb_R = 1<<AWB_BITS; awb_G = 1<<AWB_BITS; awb_B = 1<<AWB_BITS;
    choosematrix(ccm,awb_R,awb_B);
    Texp = 8; /* in ms */
    Gain = MIN_GAIN; /* from MIN_GAIN...MAX_GAIN */
    focus_direction = 1;
	focus = NEUTRAL_FOCUS;
	focus_stepsize = FOCUS_MINSTEP;
    
        
    /* to be done at the start of every image */
    rsum = gsum = bsum = 0;
    
    noise_threshold = 33 - Gain + (Texp>>2);
    /* set threshold according to exposure time and gain */
    if (noise_threshold < 16)
        noise_threshold = 16;    
    
      /* code changed here!!! change is: 5th row initialization done seperately(short row) */
    /* initialize buffer(row1-4) to all zero */
    for (r=0; r<IMAGE_LINES_IN_BUFFER-1;r++)
        for (c=0; c<PIXEL_ARRAY_WIDTH; c++)
            buffer[r][c] = 0;
    
	/* initialize 5th row */
	 for (c=0; c<5; c++)
            buffer[4][c] = 0;
    
    edginess = 0;
    edginess_window = 0;
    /* per pixel operations */
    for (r=0; r<PIXEL_ARRAY_HEIGHT; r++)
    {
        for (c=0; c<PIXEL_ARRAY_WIDTH; c++) 
        {
            if (input == TESTPATTERN)    /* mux between test pattern and simulation image */
                pixel = test_pattern_generator(); 
            else
                pixel = I[r][c];
            
            pixel = black_level_subtract(pixel,CALIBRATED_BLACK_LEVEL);
            
            pixel = lens_shading_correction(pixel, row_prf[r], col_prf[c]);
            
            pixel = awb_sums_and_cga(pixel, r, c, awb_R, awb_G, awb_B, &rsum, &gsum, &bsum); 
            /* input is pixel, row, col, awb gains, output is red sum, green sum, blue sum */
            
            update_buffer(pixel, buffer); /* store pixel in buffer */
            
            /* combine color interpolation and noise reduction, waiting until buffer is suitably full */
            if (r >= BUFFER_OFFSET) 
            {
                colorinterpolate_noisereduce(buffer,r-BUFFER_OFFSET,c,noise_threshold,&red,&green,&blue);
            } else {
                red = green = blue = pixel;  /* gray out. These are not used any way */
            }
            
            /* correct from sensor color to SRGB */
            colorcorrect(ccm,&red,&green,&blue);
            
            red = gammacorrect_LUT(red);      /* display gamma correction by look up table -- output will be 8 bits */
			green = gammacorrect_LUT(green);  
            blue = gammacorrect_LUT(blue); 
            
            prevy = y;
            rgb2yuv(red,green,blue,&y,&u,&v);
            edginess = edginess + absdiff16(prevy,y);
            
            if (r > TOP_AF_BOUNDARY && r < BOTTOM_AF_BOUNDARY && c > LEFT_AF_BOUNDARY && c < RIGHT_AF_BOUNDARY)
                edginess_window = edginess_window + absdiff16(prevy,y);            /* no need for separate edginess function */
            
            /* for simulation, keep the red, green, blue, and y, u, v values in a big matrix */
            if (r>= BUFFER_OFFSET  && c > 1 && c < PIXEL_ARRAY_WIDTH-2) 
            { 
            	r2 = r-BUFFER_OFFSET; c2 = c-2;       
                Ic[r2][c2][0] = red; Ic[r2][c2][1] = green; Ic[r2][c2][2] = blue;
                Iyuv[r2][c2][0] = y; Iyuv[r2][c2][1] = u;   Iyuv[r2][c2][2] = v;
                /* in ASIC we simply output r,g,b or y,u,v as needed, without storing */
            }
               
        }
    }
    
    /* per frame operations, done in ASIC: auto white balance (awb), auto-exposure (AE), and autofocus (AF) */
    awb(rsum,gsum,bsum,&awb_R,&awb_G,&awb_B);  /* awb sets gains in Cam */
    printf("awb output: R=%d, G=%d, B=%d\n",awb_R,awb_G,awb_B);
    
    choosematrix(ccm,awb_R,awb_B);
    
    autoexposure(rsum,gsum,bsum,&Gain,&Texp);  /* sets exposure time for next frame */
    printf("ae output: Texp=%d Gain=%d\n",Texp,Gain);
    
    /* autofocus */
    focus=autofocus(focus, edginess_window, &prev_edginess, &focus_direction, focus_stepsize);  
    printf("af output: E=%d EW=%d and focus setting %d\n",edginess,edginess_window,focus);
    
    /* now convert RGB image to output depending on color format desired */
    switch (output) {
            
        case YUV444:
            /* YUV format */
            printf("Output in YUV 444 format\n");
            writeppm("outputyuv.ppm",Iyuv);	
            break;
        case YCbCr422:
            /* rgb2ycbcr 422 format */
            printf("Output in YCbCr 422 format\n");
            Iycc = ui16colorim(0,HEIGHT-1,0,WIDTH-1);
            rgb2ycbcr422(Ic,Iycc);
            writeppm("outputycbcr422.ppm",Iycc);
            free_ui16colorim(Iycc,0,HEIGHT-1,0,WIDTH-1);
            break;
        case RGB565:
            printf("Output in RGB 565 format\n");
            Irgb565 = ui16matrix(0,HEIGHT-1,0,WIDTH-1);
            rgb2rgb565(Ic,Irgb565);
            inputpgm->matrix = Irgb565;
            outputpgm = inputpgm;
            writePGM("outputrgb565.ppm",outputpgm);
            free_ui16matrix(Irgb565,0,HEIGHT-1,0,WIDTH-1);
            break;
        case ITUBT656:
            printf("Output in ITU BT 565 format\n");
            Ibtu = ui16colorim(0,HEIGHT-1,0,WIDTH-1);
            rgb2ycbcr422(Ic,Ibtu);
            /* need to write each field separately -- but that requires a full frames store */
            writebtu656("outputbtu",Ibtu,field);
            free_ui16colorim(Ibtu,0,HEIGHT-1,0,WIDTH-1);
            if (field == 0) 
                field = 1;
            else 
                field = 0;
            break;
        case RGB444:
        default:
            printf("Output in RGB 444 format\n");
            writeppm("output.ppm",Ic);
    }
    
    /* end of code, wrap up */
    
    /* free allocated memory */
    free_ui16vector(row_prf,0,PIXEL_ARRAY_HEIGHT-1);
    free_ui16vector(col_prf,0,PIXEL_ARRAY_WIDTH-1); 
    
    
    /*  free_ui16matrix(buffer,0,IMAGELINESINBUFFER-1,0,Cam.W-1); */
    free_ui16colorim(Iyuv,0,HEIGHT-1,0,WIDTH-1);
    free_ui16colorim(Ic,0,HEIGHT-1,0,WIDTH-1);
    free_ui16matrix(I,0,HEIGHT-1,0,WIDTH-1); 
    free(inputpgm);
    
    return 0;
}

