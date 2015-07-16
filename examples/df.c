#include <sys/statvfs.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <mntent.h>
#include <m_list.h>
#include <its.h>

#include "src/tabl.h"

int
main(void)
{
	struct tabl t;
	struct statvfs vfs;
	FILE *file;
	struct mntent *mount;
	struct m_list values;
	unsigned long total;
	unsigned long available;
	unsigned long _free;
	unsigned long used;
	unsigned long final;
	char* final_str;

	tabl_init(&t, 0);
	tabl_add_column(&t, "Mount point", NULL, TABL_ALIGN_LEFT);
	tabl_add_column(&t, "Free space", "%", TABL_ALIGN_LEFT);

	m_list_init(&values);
	file = setmntent("/etc/mtab", "r");
	while ((mount = getmntent(file)) != NULL) {
		statvfs(mount->mnt_dir, &vfs);
		total = vfs.f_blocks * vfs.f_frsize / 1024;
		available = vfs.f_bavail * vfs.f_frsize / 1024;
		_free = vfs.f_bfree * vfs.f_frsize / 1024;
		used = total - _free;

		if (used + available == 0)
			continue;

		final = 100 - 100 * used / (used + available);
		final_str = its(&final, sizeof(unsigned long)*8, ITS_UNSIGNED, ITS_BASE_DEC);

		m_list_clear(&values);
		m_list_append(&values, M_LIST_COPY_DEEP, mount->mnt_dir, strlen(mount->mnt_dir)+1);
		m_list_append(&values, M_LIST_COPY_DEEP, final_str, strlen(final_str)+1);
		tabl_add_row(&t, &values);

		free(final_str);
	}
	endmntent(file);

	tabl_render(&t, NULL);
	return EXIT_SUCCESS;
}

