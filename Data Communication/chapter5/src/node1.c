#include <stdio.h>

#define N 1
#define ALTER_SEQNUM(X) X##1

int ALTER_SEQNUM(connectcosts)[4] = { 1,  0,  1, 999 };

#include "repition.c"