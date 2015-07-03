#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <m_list.h>

#include "src/tabl.h"

int
main(int argc, char* argv[])
{
	struct passwd* pwd;
	struct tabl t;
	struct m_list values;
	unsigned int width;

	switch (argc) {
		case 1:
			width = 0;
		break;

		case 2:
			width = atoi(argv[1]);
		break;

		default:
			fprintf(stderr, "Wrong arguments.\nUsage: passwd [width]\n");
		return EXIT_FAILURE;
	}

	tabl_init(&t, width);
	tabl_add_column(&t, "UID", TABL_CONTENT_DECIMAL, TABL_ALIGN_RIGHT);
	tabl_add_column(&t, "Name", TABL_CONTENT_STRING, TABL_ALIGN_LEFT);
	tabl_add_column(&t, "Directory", TABL_CONTENT_STRING, TABL_ALIGN_LEFT);
	tabl_add_column(&t, "Shell", TABL_CONTENT_STRING, TABL_ALIGN_LEFT);

	m_list_init(&values);
	while ((pwd = getpwent()) != NULL) {
		m_list_clear(&values);
		m_list_append(&values, M_LIST_COPY_DEEP, &pwd->pw_uid, sizeof(uid_t));
		m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_name, strlen(pwd->pw_name)+1);
		m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_dir, strlen(pwd->pw_dir)+1);
		m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_shell, strlen(pwd->pw_shell)+1);
		tabl_add_row(&t, &values);
	}
	endpwent();

	tabl_render(&t, NULL);
	return EXIT_SUCCESS;
}

