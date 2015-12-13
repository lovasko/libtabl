#include <math.h>
#include <string.h>

#include "tabl.h"
#include "column.h"

static size_t
max(size_t a, size_t b)
{
	return (a > b ? a : b);
}

int
tabl_add_column(struct tabl* t,
                const char* name,
                const char* suffix,
                int align)
{
	struct column col;
	uint64_t row_count;

	if (t == NULL)
		return TABL_E_NULL;

	m_list_length(&t->rows, &row_count);
	if (row_count > 0)
		return TABL_E_ROWS;

	col.name = strdup(name);
	col.suffix = (suffix != NULL ? strdup(suffix) : NULL);
	col.suffix_width = (suffix != NULL ? (unsigned int)strlen(suffix) : 0);
	col.align = align;
	col.width = (unsigned int)max(strlen(name), col.suffix_width);
	col.newline = 0;

	m_list_append(&t->columns, M_LIST_COPY_DEEP, &col, sizeof(struct column));
	return TABL_OK;
}

static void
extend_width(void* _col, void* value, void* payload)
{
	struct column* col;
	unsigned int width;

	(void)payload;

	col = _col;
	width = (unsigned int)strlen((char*)value) + col->suffix_width;

	if (width > col->width)
		col->width = width;
}

int
tabl_add_row(struct tabl* t, struct m_list* values)
{
	uint64_t column_count;
	uint64_t values_count;
	struct m_list* row;

	m_list_length(&t->columns, &column_count);
	m_list_length(values, &values_count);

	if (column_count != values_count)
		return TABL_E_COUNT;

	m_list_zip(&t->columns, values, extend_width, NULL);

	row = malloc(sizeof(struct m_list));
	m_list_init(row);
	m_list_copy(values, row, M_LIST_COPY_DEEP);
	m_list_append(&t->rows, M_LIST_COPY_SHALLOW, row, 0);

	return TABL_OK;
}

