#ifndef TABL_H
#define TABL_H

#include <stdio.h>
#include <m_list.h>

typedef struct tabl {
	m_list columns;
	m_list rows;
	unsigned int max_width;
	unsigned int padding;
} tabl;

#define TABL_OK      0
#define TABL_E_NULL  1
#define TABL_E_COUNT 2
#define TABL_E_ROWS  3

#define TABL_ALIGN_LEFT  0
#define TABL_ALIGN_RIGHT 1

int tabl_init(tabl* t, unsigned int max_width);
int tabl_add_column(tabl* t, const char* name, const char* suffix, int align);
int tabl_add_row(tabl* t, m_list* values);
int tabl_sort(tabl* t, unsigned int col_num, int(*cmp_fn)(void*, void*));
int tabl_render(tabl* t, FILE* file);

#endif

