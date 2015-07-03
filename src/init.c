#include "tabl.h"

int
tabl_init(struct tabl* t, size_t max_width)
{
	if (t == NULL)
		return TABL_E_NULL;

	m_list_init(&t->columns);
	m_list_init(&t->rows);
	t->max_width = max_width;

	return TABL_OK;
}

