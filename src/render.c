#include <stdio.h>

#include "tabl.h"
#include "column.h"

static void
render_column(void* _col, void* out)
{
	struct column* col;
	
	col = _col;
	if (col->newline)
		fprintf((FILE*)out, "\n ");

	fprintf((FILE*)out, col->align == TABL_ALIGN_LEFT ? "%-*s " : "%*s ",
	                    col->width,
	                    col->name);
}

static void
render_value(void* _col, void* value, void* out)
{
	struct column* col;

	col = _col;
	if (col->newline)
		fprintf((FILE*)out, "\n ");

	switch (col->content) {
		case TABL_CONTENT_STRING:
			fprintf((FILE*)out, col->align == TABL_ALIGN_LEFT ? "%-*s " : "%*s ",
			                    col->width,
			                    (char*)value);
		return;

		case TABL_CONTENT_DECIMAL:
			fprintf((FILE*)out, col->align == TABL_ALIGN_LEFT ? "%-*d " : "%*d",
			                    col->width,
													*((int*)value));
		return;
	}
}

static void
render_row(void* row, void* t, void* out)
{
	m_list_zip(&((struct tabl*)t)->columns, row, render_value, out);
	fprintf((FILE*)out, "\n");
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
tabl_render(struct tabl* t, FILE* file)
{
	unsigned int accum;
	FILE* out;

	if (t == NULL)
		return TABL_E_NULL;

	out = (file == NULL ? stdout : file);

	if (t->max_width != 0) {
		accum = 0;
		m_list_map2(&t->columns, set_newline, &accum, &t->max_width);
	}

	m_list_map(&t->columns, render_column, out);
	fprintf(out, "\n");
	m_list_map2(&t->rows, render_row, t, out);

	return TABL_OK;
}

