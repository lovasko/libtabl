#ifndef TABL_COLUMN_H
#define TABL_COLUMN_H

#include <stdint.h>

struct column
{
	char* name;
	uint8_t content;
	uint8_t align;
	unsigned int max_width;
};

#endif

