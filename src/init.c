#include "tabl.h"

int
tabl_init(struct tabl* t)
{
	if (t == NULL)
		return TABL_E_NULL;

	m_list_init(&t->columns);
	m_list_init(&t->rows);

	return TABL_OK;
}

