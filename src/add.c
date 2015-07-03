#include <math.h>
#include <string.h>

#include "tabl.h"
#include "column.h"

int
tabl_add_column(struct tabl* t, const char* name, uint8_t content, uint8_t align)
{
	struct column col;
	uint64_t row_count;

	if (t == NULL)
		return TABL_E_NULL;

	m_list_length(&t->rows, &row_count);
	if (row_count > 0)
		return TABL_E_ROWS;

	col.name = strdup(name);
	col.content = content;
	col.align = align;
	col.max_width = strlen(name);

	m_list_append(&t->columns, M_LIST_COPY_DEEP, &col, sizeof(struct column));
	return TABL_OK;
}

static void
extend_width(void* _col, void* value, void* payload)
{
	struct column* col;
	unsigned int width;

	(void)payload;

	width = 0;
	col = _col;
	switch (col->content) {
		case TABL_CONTENT_STRING:
			width = strlen(value);
		break;

		case TABL_CONTENT_DECIMAL:
			if (*((int*)value) == 0)
				width = 1;
			else 
				width = (unsigned int)floor(log10(fabs(*((int*)value))))+1;
		break;
	}
	
	if (width > col->max_width)
		col->max_width = width;
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

