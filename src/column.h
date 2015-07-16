#ifndef TABL_COLUMN_H
#define TABL_COLUMN_H

#include <stdint.h>

struct column {
	char* name;
	char* suffix;
	unsigned int suffix_width;
	unsigned int width;
	int newline;
	uint8_t align;
};

#endif

