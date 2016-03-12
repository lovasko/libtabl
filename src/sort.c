#include "tabl.h"

struct sort_arg {
	int(*cmp_fn)(void*, void*);
	unsigned int col_num;
	unsigned int padding;
};

static void
build_index(void* row, void* payload)
{
	(void)payload;
	m_list_build_index((m_list*)row);
}

static void
drop_index(void* row, void* payload)
{
	(void)payload;
	m_list_drop_index((m_list*)row);
}

static int
compare_rows(void* a, void* b, void* payload)
{
	m_list* list_a;
	m_list* list_b;
	m_list_elem* elem_a;
	m_list_elem* elem_b;
	void* data_a;
	void* data_b;
	struct sort_arg* arg;

	list_a = a;
	list_b = b;
	arg = payload;

	m_list_nth(list_a, (uint64_t)arg->col_num, &elem_a);
	m_list_nth(list_b, (uint64_t)arg->col_num, &elem_b);

	m_list_elem_data(elem_a, &data_a);
	m_list_elem_data(elem_b, &data_b);

	return arg->cmp_fn(data_a, data_b);
}

int
tabl_sort(tabl* t, unsigned int col_num, int(*cmp_fn)(void*, void*))
{
	struct sort_arg arg;

	m_list_map(&t->rows, build_index, NULL);

	arg.col_num = col_num;
	arg.cmp_fn = cmp_fn;
	m_list_sort(&t->rows, compare_rows, &arg);

	m_list_map(&t->rows, drop_index, NULL);

	return TABL_OK;
}

