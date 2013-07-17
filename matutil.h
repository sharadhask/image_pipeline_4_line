/*
//  matutil.h
//  ipipe
//
//  Created by Ramakrishna  Kakarala on 15/4/13.
//  Copyright 2013 Nanyang Technological University. All rights reserved.
*/

#ifndef ipipe_matutil_h
#define ipipe_matutil_h

void iperror(char error_text[]);
unsigned short *ui16vector(long nl, long nh);
unsigned short **ui16matrix(long nrl, long nrh, long ncl, long nch);
unsigned short ***ui16colorim(long nrl, long nrh, long ncl, long nch);
void free_ui16vector(unsigned short *v, long nl, long nh);
void free_ui16matrix(unsigned short **m, long nrl, long nrh, long ncl, long nch);
void free_ui16colorim(unsigned short ***m, long nrl, long nrh, long ncl, long nch);

#endif
