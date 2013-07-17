/*
//  writeppm.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 19/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include "ipipe.h"
#include <stdio.h>
#include <stdlib.h>

void writeppm(const char *filename, uint16_t ***Ic)
{
    uint16_t r, c;
    FILE *fp = fopen(filename, "w"); /* b - binary mode */
    if (fp == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }

    (void) fprintf(fp, "P3\n%d %d\n%d", WIDTH,HEIGHT,255);
    for (r = 0; r < HEIGHT; r++)
    {
        fprintf(fp,"\n");
        for (c = 0; c < WIDTH; c++)
        {
            fprintf(fp,"%d %d %d ",Ic[r][c][0],Ic[r][c][1],Ic[r][c][2]);  
        }
    }
    fclose(fp);
    
}