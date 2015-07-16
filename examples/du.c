#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <m_list.h>
#include <its.h>

#include "src/tabl.h"

int
main(int argc, char* argv[])
{
	DIR* dir;
	FILE* output;
	int size;
	struct dirent* de;
	struct m_list values;
	struct stat st;
	struct tabl t;
	char* size_str;

	if ((dir = opendir(".")) == NULL) {
		fprintf(stderr, "Unable to open '.'.\n");
		return EXIT_FAILURE;
	}

	switch (argc) {
		case 1:
			output = NULL;
		break;

		case 2:
			if ((output = fopen(argv[1], "w")) == NULL) {
				fprintf(stderr, "Unable to open the file '%s'.\n", argv[1]);
				return EXIT_FAILURE;
			}
		break;

		default:
			fprintf(stderr, "Wrong arguments.\nUsage: du [path]\n");
		return EXIT_FAILURE;
	}
	
	tabl_init(&t, 0);
	tabl_add_column(&t, "File", NULL, TABL_ALIGN_LEFT);
	tabl_add_column(&t, "Size", NULL, TABL_ALIGN_LEFT);

	m_list_init(&values);
	do {
		if ((de = readdir(dir)) != NULL) {
			stat(de->d_name, &st);
			size = (int)st.st_size;
			size_str = its(&size, ITS_SIZE_INT, ITS_SIGNED, ITS_BASE_DEC);
					
			m_list_clear(&values);
			m_list_append(&values, M_LIST_COPY_DEEP, de->d_name, strlen(de->d_name)+1);
			m_list_append(&values, M_LIST_COPY_DEEP, size_str, strlen(size_str)+1);
			tabl_add_row(&t, &values);

			free(size_str);
		}
	} while (de != NULL);

	tabl_render(&t, output);
	return EXIT_SUCCESS;
}

