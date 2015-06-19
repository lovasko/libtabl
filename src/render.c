#include <stdio.h>

#include "tabl.h"
#include "column.h"

static void
render_column(void* _col, void* payload)
{
	struct column* col;
	
	(void)payload;
	col = _col;
	printf(col->align == TABL_ALIGN_LEFT ? "%-*s " : "%*s ",
	       col->max_width,
	       col->name);
}

static void
render_value(void* _col, void* value, void* payload)
{
	struct column* col;

	(void)payload;
	col = _col;
	switch (col->content) {
		case TABL_CONTENT_STRING:
			printf(col->align == TABL_ALIGN_LEFT ? "%-*s " : "%*s ",
			       col->max_width,
			       (char*)value);
		return;

		case TABL_CONTENT_DECIMAL:
			printf("%*d ", col->max_width, *((int*)value));
		return;
	}
}

static void
render_row(void* row, void* t)
{
	m_list_zip(&((struct tabl*)t)->columns, row, render_value, NULL);
	printf("\n");
}

int
tabl_render(struct tabl* t)
{
	if (t == NULL)
		return TABL_E_NULL;

	m_list_map(&t->columns, render_column, NULL);
	printf("\n");
	m_list_map(&t->rows, render_row, t);

	return TABL_OK;
}

