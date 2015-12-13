#ifndef TABL_COLUMN_H
#define TABL_COLUMN_H

struct column {
	char* name;
	char* suffix;
	unsigned int suffix_width;
	unsigned int width;
	int align;
	int newline;
};

#endif

