#ifndef __MASTER_HEADER_H
#define __MASTER_HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <string>


#define PI M_PI

typedef unsigned int uint;
typedef unsigned char uchar;


#define DEBUG 0
#define __BITACORA \
		printf("bitacora:\tLINEA %d\tARCHIVO %s\n", __LINE__, __FILE__);\
		fflush(stdout);

#endif
