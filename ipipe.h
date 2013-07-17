/*
//  ipipe.h
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 15/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#ifndef ipipe_ipipe_h
#define ipipe_ipipe_h

/********** constants *****************************/

#define VERSION 0.5
#define BITS_PER_PIXEL 10
#define PIXELMASK ((1<<BITS_PER_PIXEL)-1)
#define MAX_PIXEL_VALUE ((1<<BITS_PER_PIXEL)-1)
#define FIRST_COLOR_RED 1         /* Bayer pattern is GR;BR */
#define PIXEL_ARRAY_WIDTH 724     /* 720 pixels + 2 on each side for interpolation */
#define PIXEL_ARRAY_HEIGHT 484    /* 480 pixels + 2 on each side for interpolation */
#define WIDTH 720  
#define HEIGHT 480 
/**** autofocus boundaries should be in registers *****/
#define LEFT_AF_BOUNDARY 180      /* autofocus uses a window */
#define RIGHT_AF_BOUNDARY 540
#define TOP_AF_BOUNDARY 180 
#define BOTTOM_AF_BOUNDARY 540 
/*****autofocus window above **************************/
#define LOG2NPIX 18               /* log2(724*484), rounded down */
#define TARGET_MEAN (((MAX_PIXEL_VALUE>>1)*PIXEL_ARRAY_WIDTH*PIXEL_ARRAY_HEIGHT)>>LOG2NPIX)
/* this is the target for the autoexposure mean calculation, roughly 684
   this is obtained by (512*724*484)>>18 */
#define CALIBRATED_BLACK_LEVEL 1  /* needs to be set after calibration */
#define LENS_SHADING_BITS 10      /* 10 bit lens shading profile */
#define PROFILE_CALIBRATION 0     /* needs to be set after sensor calibration */
#define AWB_BITS 5                /* from 1...32 and up */
#define LOW_AWB_THRESHOLD 6

/***********color correction settings*************/
#define DAYLIGHT_R 32
#define DAYLIGHT_B 32
#define FLUORESCENT_R 24
#define FLUORESCENT_B 40
#define INCANDESCENT_R 40
#define INCANDESCENT_B 24
#define CLOUDY_R 36
#define CLOUDY_B 28

/************gamma correction settings for 10 bit image ************/
#define BREAK_PT1 127
#define SLOPE1 32
#define BREAK_PT2 255  /* go in steps of 128 */
#define SLOPE2 32
#define BREAK_PT3 383  /* 256 + 127 */
#define SLOPE3 32
#define BREAK_PT4 511 
#define SLOPE4 32
#define BREAK_PT5 639  
#define SLOPE5 32
#define BREAK_PT6 767
#define SLOPE6 32

/************auto exposure settings ****************/
#define MIN_EXPOSURE 2  /* ms */
#define MAX_EXPOSURE 100 
#define MIN_GAIN 1
#define MAX_GAIN 16

/***********sharpening settings ******************/
#define SHARPENING_FACTOR 2 /* the smaller the sharper */

/************auto focus settings ******************/
#define FOCUS_AT_INFINITY 32
#define NEUTRAL_FOCUS 16
#define FOCUS_MINSTEP 1
#define MACRO_FOCUS 0

/************asic settings ***********************/
#define IMAGE_LINES_IN_BUFFER 5 
#define BUFFER_OFFSET (IMAGE_LINES_IN_BUFFER - 1)
#define MIDLINE 2     /* should be IMAGELINESINBUFFER/2 */

/********** typedef *******************************/
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned char uint8_t;
typedef enum _OutputFormat {
	YUV444,
	YCbCr422,
	RGB565,
	RGB444,     /* 8 bits/pixel */
	ITUBT656
}OutputFormat;

typedef enum _InputFormat {
    TESTPATTERN,
    PIXELARRAY,
}InputFormat;

typedef struct _PGMData {
    uint16_t height;
    uint16_t width;
    uint16_t max_gray;
    uint16_t **matrix;
} PGMData;

typedef struct _Gamma {
	uint16_t slopes[6];
	uint16_t break_pts[6];
} Gamma_curve;


/********** macros ********************************/
/* for pgm read and write */
#define HI(num) (((num) & 0xFF00) >> 8)
#define LO(num) ((num) & 0x00FF)

/********** prototypes ***************************/
void print_config(void);

uint16_t test_pattern_generator(void);
PGMData* readPGM(const char *file_name, PGMData *data);
void writePGM(const char *filename, const PGMData *data);
uint16_t black_level_subtract(uint16_t pixel, uint16_t black_level);
void construct_lens_profile(uint16_t calibration, uint16_t *rp, uint16_t *cp); 
uint16_t lens_shading_correction(uint16_t pixel, uint16_t rp, uint16_t cp); 
void awb(uint32_t rsum,uint32_t gsum,uint32_t bsum, uint16_t *awb_R, uint16_t *awb_G, uint16_t *awb_B); 
void autoexposure(uint32_t rsum, uint32_t gsum, uint32_t bsum, uint16_t *Gain, uint16_t *Texp);
void colorinterpolate_noisereduce(uint16_t *buffer[5], uint16_t r, uint16_t c, uint16_t noise_threshold,
                      uint16_t *red, uint16_t *green, uint16_t *blue);

void writeppm(const char *filename, uint16_t ***Ic);
uint16_t absdiff16(uint16_t x, uint16_t y);
void colorcorrect(int16_t ccm[3][3], uint16_t *red, uint16_t *green, uint16_t *blue);
void choosematrix(int16_t ccm[3][3], uint16_t awb_R, uint16_t awb_B);
uint16_t gammacorrect_LUT(uint16_t inpval);

/* asic */
uint16_t black_level_subtract(uint16_t pixel, uint16_t black_level);
void update_buffer(uint16_t pixel, uint16_t *buffer[5]);
uint16_t awb_sums_and_cga(uint16_t pixel, uint16_t r, uint16_t c, uint16_t awb_R, uint16_t awb_G, uint16_t awb_B, 
					  uint32_t *rsum, uint32_t *gsum, uint32_t *bsum);
uint16_t noise_reduce(uint16_t *buffer[5], uint16_t row, uint16_t col, uint16_t threshold,int count_w,int count_col);

/* outputs */
void rgb2yuv(uint16_t r, uint16_t g, uint16_t b, uint16_t *y, uint16_t *u, uint16_t *v);
void rgb2ycbcr422(uint16_t ***Ic, uint16_t ***Iycc);
void rgb2rgb565(uint16_t ***Ic, uint16_t **I565);
void writebtu656(const char *filename, uint16_t ***Iycc, uint16_t field);

uint16_t autofocus(uint16_t focus, uint32_t edginess, uint32_t *prev_edginess, int16_t *focus_direction, uint16_t focus_stepsize);


#endif
