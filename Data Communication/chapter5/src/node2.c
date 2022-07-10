#include <stdio.h>

#define N 2
#define ALTER_SEQNUM(X) X##2

int ALTER_SEQNUM(connectcosts)[4] = { 3,  1,  0,  2 };

#include "repition.c"