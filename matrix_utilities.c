/*
//  matrix_utilities.c
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 15/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "matutil.h"

#define NR_END 1
#define FREE_ARG char*

void iperror(char error_text[])
/* Ipipe standard error handler */
{
	fprintf(stderr,"Ipipe run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}

unsigned short *ui16vector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
	unsigned short *v;
    
	v=(unsigned short *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(unsigned short)));
	if (!v) iperror("allocation failure in ui16vector()");
	return v-nl+NR_END;
}

unsigned short **ui16matrix(long nrl, long nrh, long ncl, long nch)
/* allocate an unsigned short matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	unsigned short **m;
    
	/* allocate pointers to rows */
	m=(unsigned short **) malloc((size_t)((nrow+NR_END)*sizeof(unsigned short *)));
	if (!m) iperror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;
    
    
	/* allocate rows and set pointers to them */
	m[nrl]=(unsigned short *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(unsigned short)));
	if (!m[nrl]) iperror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;
    
	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
	/* return pointer to array of pointers to rows */
	return m;
}

unsigned short ***ui16colorim(long nrl, long nrh, long ncl, long nch)
/* allocate an unsigned short matrix with range m[nrl..nrh][ncl..nch][0..2] */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=3;
	unsigned short ***t;
    
	/* allocate pointers to pointers to rows */
	t=(unsigned short ***) malloc((size_t)((nrow+NR_END)*sizeof(unsigned short**)));
	if (!t) iperror("allocation failure 1 in ui16colorim()");
	t += NR_END;
	t -= nrl;
    
	/* allocate pointers to rows and set pointers to them */
	t[nrl]=(unsigned short **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(unsigned short*)));
	if (!t[nrl]) iperror("allocation failure 2 in ui16colorim()");
	t[nrl] += NR_END;
	t[nrl] -= ncl;
    
	/* allocate rows and set pointers to them */
	t[nrl][ncl]=(unsigned short *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(unsigned short)));
	if (!t[nrl][ncl]) iperror("allocation failure 3 in ui16colorim()");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= 0;
    
	for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for(i=nrl+1;i<=nrh;i++) {
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}
    
	/* return pointer to array of pointers to rows */
	return t;
}

void free_ui16vector(unsigned short *v, long nl, long nh)
/* free an unsigned short vector allocated with ui16vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_ui16matrix(unsigned short **m, long nrl, long nrh, long ncl, long nch)
/* free an int matrix allocated by imatrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}
void free_ui16colorim(unsigned short ***t, long nrl, long nrh, long ncl, long nch)
/* free a ui16colorim matrix */
{
	free((FREE_ARG) (t[nrl][ncl]+0-NR_END));
	free((FREE_ARG) (t[nrl]+ncl-NR_END));
	free((FREE_ARG) (t+nrl-NR_END));
}
