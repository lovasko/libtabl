#include <stdio.h>

#include "tabl.h"
#include "column.h"

static void
render_column(void* _col, void* payload)
{
	struct column* col;
	
	(void)payload;
	col = _col;

	if (col->newline)
		printf("\n ");

	printf(col->align == TABL_ALIGN_LEFT ? "%-*s " : "%*s ",
	       col->width,
	       col->name);
}

static void
render_value(void* _col, void* value, void* payload)
{
	struct column* col;

	(void)payload;
	col = _col;

	if (col->newline)
		printf("\n ");

	switch (col->content) {
		case TABL_CONTENT_STRING:
			printf(col->align == TABL_ALIGN_LEFT ? "%-*s " : "%*s ",
			       col->width,
			       (char*)value);
		return;

		case TABL_CONTENT_DECIMAL:
			printf("%*d ", col->width, *((int*)value));
		return;
	}
}

static void
render_row(void* row, void* t)
{
	m_list_zip(&((struct tabl*)t)->columns, row, render_value, NULL);
	printf("\n");
}

static void
set_newline(void* _col, void* accum, void* max_width)
{
	struct column* col;

	col = _col;
	if (*((unsigned int*)accum) + col->width > *((unsigned int*)max_width)) {
		col->newline = 1;
		*((unsigned int*)accum) = 1;
	}

	*((unsigned int*)accum) += col->width;
}

int
tabl_render(struct tabl* t)
{
	unsigned int accum;

	if (t == NULL)
		return TABL_E_NULL;

	if (t->max_width != 0) {
		accum = 0;
		m_list_map2(&t->columns, set_newline, &accum, &t->max_width);
	}

	m_list_map(&t->columns, render_column, NULL);
	printf("\n");
	m_list_map(&t->rows, render_row, t);

	return TABL_OK;
}

