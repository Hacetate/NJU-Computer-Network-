#include <stdio.h>

#define N 0
#define ALTER_SEQNUM(X) X##0

int ALTER_SEQNUM(connectcosts)[4] = { 0 , 1 , 3, 7 };

#include "repition.c"