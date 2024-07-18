#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>

#define len(x) (sizeof(x)/sizeof(x[0]))
#define duty(x) (uint32_t)((float)100/(float)x)

typedef enum {
	OFF = 0,
	ON = 1
}ledState_t;

#endif
