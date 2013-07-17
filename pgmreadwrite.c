/*
//  pgmreadwrite.c from ugurkoltuk.wordpress.com/2010/03/04/an-extreme-simple-pgm-io-api/
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 15/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/ 

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "matutil.h"
#include "ipipe.h"

static void SkipComments(FILE *fp);

static void SkipComments(FILE *fp)
{
    int ch;
    char line[100];
    
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;
    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        SkipComments(fp);
    } else
        fseek(fp, -1, SEEK_CUR);
}

/*for reading an ascii format pgm file:*/
PGMData* readPGM(const char *file_name, PGMData *data)
{
    FILE *pgmFile;
    char version[3];
    int i, j, x;
    
    pgmFile = fopen(file_name, "rt");
    if (pgmFile == NULL) {
        perror("cannot open file to read");
        exit(EXIT_FAILURE);
    }
    
    fgets(version, sizeof(version), pgmFile);
    if (strcmp(version, "P2")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }
    
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &x);
    data->width = (uint16_t) x;
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &x);
    data->height = (uint16_t) x;
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &x);
    data->max_gray = (uint16_t) x;
    data->matrix = ui16matrix(0, data->height-1, 0, data->width-1);
    
    for (i=0; i<data->height; i++) 
    {
        for (j=0; j<data->width; j++)
        {
            fscanf(pgmFile,"%d",&x);
            data->matrix[i][j] = x;
        }
       
    }
    
    fclose(pgmFile);
    return data;
    
}

/*and for writing in ascii */

void writePGM(const char *filename, const PGMData *data)
{
    FILE *pgmFile;
    int i, j;
    
    pgmFile = fopen(filename, "w");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }
    
    fprintf(pgmFile, "P2\n");  /* ascii gray map */
    fprintf(pgmFile, "%d %d\n", data->width, data->height);
    fprintf(pgmFile, "%d ", data->max_gray);
    
    for (i = 0; i < data->height; i++)
    {
        fprintf(pgmFile,"\n");
        for (j = 0; j < data->width; j++) 
        {
            fprintf(pgmFile,"%d ",data->matrix[i][j]);
        } 
        
    }
          
    fclose(pgmFile);
}
