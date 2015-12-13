#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <m_list.h>
#include <its.h>

#include "src/tabl.h"

int
main(int argc, char** argv)
{
	struct passwd* pwd;
	struct tabl t;
	struct m_list values;
	unsigned int width;
	char* uid_str;

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
	tabl_add_column(&t, "UID", NULL, TABL_ALIGN_RIGHT);
	tabl_add_column(&t, "Name", NULL, TABL_ALIGN_LEFT);
	tabl_add_column(&t, "Directory", NULL, TABL_ALIGN_LEFT);
	tabl_add_column(&t, "Shell", NULL, TABL_ALIGN_LEFT);

	m_list_init(&values);
	while ((pwd = getpwent()) != NULL) {
		uid_str = its(&pwd->pw_uid, sizeof(uid_t)*8, ITS_UNSIGNED, ITS_BASE_DEC);

		m_list_clear(&values);
		m_list_append(&values, M_LIST_COPY_DEEP, uid_str, strlen(uid_str)+1);
		m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_name, strlen(pwd->pw_name)+1);
		m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_dir, strlen(pwd->pw_dir)+1);
		m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_shell, strlen(pwd->pw_shell)+1);
		tabl_add_row(&t, &values);

		free(uid_str);
	}
	endpwent();

	tabl_render(&t, NULL);
	return EXIT_SUCCESS;
}

