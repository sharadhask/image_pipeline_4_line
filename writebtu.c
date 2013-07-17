/*
//  writebtu.c
// Write output in BTU 656 format for noninterlaced 8 bit formats
//
//  Created by Ramakrishna  Kakarala on 19/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "ipipe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writebtu656(const char *filename, uint16_t ***Iycc, uint16_t field)
{
    uint16_t r, c;
    char fname[80];
    FILE *fp;
    
    strcpy(fname,filename);
    strcat(fname,"_btu.656");
    
    fp = fopen(fname,"wb");
    
    /* copy y component to pgm */
    for (r=field; r<HEIGHT; r+=2) 
    {
       /* write start of active video -- four 10 bit words */
       fprintf(fp,"%uh",0x3FF); /* want this to be 10 bit in hardware */
       fprintf(fp,"%uh",0x000);
       fprintf(fp,"%uh",0x000);
       if (field == 0)
       		fprintf(fp,"%uh",0x200); /* for even field */ 
       else 
       		fprintf(fp,"%uh",0x31C); /* for odd field */
    	for (c=0; c<WIDTH; c+=2) 
    	{
    	  /* write in Cb, Y, Cr, Y, Cb, Y, Cr format */
    	  fprintf(fp,"%uh", (Iycc[r][c][1]<<2));  /* this should actually be in 10-bit format */
          fprintf(fp,"%uh", (Iycc[r][c][0]<<2));  /* the 8 bit data is left justified in 10 bits */
    	  fprintf(fp,"%uh", (Iycc[r][c][2]<<2));
    	  fprintf(fp,"%uh", (Iycc[r][c+1][0]<<2));  /* odd samples only Y */
    	}
       /* write end of active video -- four 10 bit words */
       fprintf(fp,"%uh",0x3FF); /* want this to be 10 bit in hardware */
       fprintf(fp,"%uh",0x000);
       fprintf(fp,"%uh",0x000);
       if (field == 0)
       		fprintf(fp,"%uh",0x274); /* for even field */ 
       else 
       		fprintf(fp,"%uh",0x368); /* for odd field */

    		
     }
    
   fclose(fp);
    
}