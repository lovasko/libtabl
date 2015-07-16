#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <m_list.h>
#include <its.h>

#include "src/tabl.h"

struct pair {
	uint32_t value;
	char* explanation;
};

int
main(void)
{
	struct tabl t;
	struct m_list values;
	struct pair pairs[] = {
		{ 0xcafebabe, "Java bytecode class file" },
		{ 0xfaceb00c, "Facebook" },
		{ 0xba5eBa11, "baseball" },
		{	0x15abe11a, "Isabella" }
	};
	char* hex_str;
	int i;

	tabl_init(&t, 0);
	m_list_init(&values);

	tabl_add_column(&t, "Hexspeak", NULL, TABL_ALIGN_LEFT);
	tabl_add_column(&t, "English", NULL, TABL_ALIGN_LEFT);

	for (i = 0; i < 4; i++) {
		hex_str = its(&pairs[i].value, 32, ITS_UNSIGNED, ITS_BASE_HEX | ITS_PREFIX);
		m_list_clear(&values);
		m_list_append(&values, M_LIST_COPY_DEEP, hex_str, strlen(hex_str)+1);
		m_list_append(&values, M_LIST_COPY_DEEP, pairs[i].explanation, strlen(pairs[i].explanation)+1);
		tabl_add_row(&t, &values);

		free(hex_str);
	}

	tabl_render(&t, NULL);
	return EXIT_SUCCESS;
}

