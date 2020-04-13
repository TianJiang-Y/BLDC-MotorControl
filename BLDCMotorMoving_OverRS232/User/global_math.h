
#ifndef __GLOBAL_MATH_H
#define __GLOBAL_MATH_H

#include <stdio.h>
#include <math.h>

#define SYSTEM_PERIOD		((5))	//unit: ms
#define MATH_PI				((3.141592653589793))
#define INTERVAL_CONSTRAINT(x, max, min)	((x)>(max)?(max):((x)<(min)?(min):(x)))

#define ENABLE_PRINT

#ifdef ENABLE_PRINT
#define U1RS232_PRINT(format, ...)			printf(format, ##__VA_ARGS__)
#else
#define U1RS232_PRINT(format, ...)
#endif

#endif

