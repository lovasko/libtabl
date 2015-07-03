#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <m_list.h>

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
	tabl_add_column(&t, "File", TABL_CONTENT_STRING, TABL_ALIGN_LEFT);
	tabl_add_column(&t, "Size", TABL_CONTENT_DECIMAL, TABL_ALIGN_LEFT);

	m_list_init(&values);
	do {
		if ((de = readdir(dir)) != NULL) {
			stat(de->d_name, &st);
			size = (int)st.st_size;
					
			m_list_clear(&values);
			m_list_append(&values, M_LIST_COPY_DEEP, de->d_name, strlen(de->d_name)+1);
			m_list_append(&values, M_LIST_COPY_DEEP, &size, sizeof(int));
			tabl_add_row(&t, &values);
		}
	} while (de != NULL);

	tabl_render(&t, output);
	return EXIT_SUCCESS;
}

