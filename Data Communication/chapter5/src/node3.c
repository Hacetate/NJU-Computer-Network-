#include <stdio.h>

#define N 3
#define ALTER_SEQNUM(X) X##3

int ALTER_SEQNUM(connectcosts)[4] = { 7,  999,  2, 0 };

#include "repition.c"