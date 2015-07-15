#ifndef TABL_H
#define TABL_H

#include <stdint.h>
#include <stdio.h>
#include <m_list.h>

struct tabl
{
	struct m_list columns;
	struct m_list rows;
	unsigned int max_width;
};

#define TABL_OK      0
#define TABL_E_NULL  1
#define TABL_E_COUNT 2
#define TABL_E_ROWS  3

#define TABL_ALIGN_LEFT  0
#define TABL_ALIGN_RIGHT 1

int tabl_init(struct tabl* t, unsigned int max_width);
int tabl_add_column(struct tabl* t, const char* name, uint8_t align);
int tabl_add_row(struct tabl* t, struct m_list* values);
int tabl_render(struct tabl* t, FILE* file);

#endif

