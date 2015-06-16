#include "tabl.h"

static void
render_column(void* _col, void* fd)
{
	struct column* col;

	(void)payload;
	col = _col;

	dprintf(*((int*)fd), "%*s ", col->max_width, col->name);
}

static void
render_value(void* _col, void* value, void* fd)
{
	struct column* col;

	col = _col;
	switch (col->content) {
		case TABL_CONTENT_STRING:
			dprintf(*((int*)fd), "%*s ", col->max_width, (char*)value);
		return;

		case TABL_CONTENT_DECIMAL:
			dprintf(*((int*)fd), "%*d ", col->max_width, *((int*)value));
		return;
	}
}

static void
render_row(void* row, void* arg)
{
	struct render_arg* rarg;

	rarg = arg;
	m_list_zip(&arg->t->columns, row, render_value, arg);
}

int
tabl_render(struct tabl* t, int fd)
{
	struct render_arg rarg;

	if (t == NULL)
		return TABL_E_NULL;

	rarg.t = t;
	rarg.fd = fd;

	m_list_map(&t->columns, print_column, &fd);
	m_list_map(&t->rows, render_row, &rarg);

	return TABL_OK;
}

