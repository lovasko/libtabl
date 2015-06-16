# libtabl
Table layout library for command line.

## Features
 * string and integer cell content 
 * left and right cell content alignment
 * strong arity checks
 * very high-level code (no loops used!)

## Example
Create a table of user names and their IDs from `/etc/passwd`.

```C
#include <unistd.h>
#include <pwd.h>
#include <m_list.h>

int
main(void)
{
	struct passwd* pwd;
	struct tabl t;
	struct m_list values;

	tabl_init(&t);
	tabl_add_column(&t, "UID", TABL_CONTENT_DECIMAL, TABL_ALIGN_RIGHT);
	tabl_add_column(&t, "Name", TABL_CONTENT_STRING, TABL_ALIGN_RIGHT);

	m_list_init(&values);
	while ((pwd = getpwent()) != NULL) {
		m_list_clear(&values);
		m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_name, strlen(pwd->name)+1);
		m_list_append(&values, M_LIST_COPY_DEEP, &pwd->pw_uid, sizeof(uid_t));
		tabl_add_row(&t, &values);
	}
	endpwent();

	tabl_render(&t, STDOUT_FILENO);
	return EXIT_SUCCESS;
}
```

Compile & run:
```
$ clang -o passwd passwd.c -ltabl
$ ./passwd
```

## API

## Supported platforms
 * FreeBSD 10.0 with Clang 3.3

If a platform does not appear to be in the previous list, it does not mean that
`libtabl` will not work in such environment. It only means that nobody tested
it - you are encouraged to do so and report either success or failure.

## Dependencies
 * [m_list](github.com/lovasko/m_list)

## Build
```
$ ninja
```

## License
2-clause BSD license. For more information please consult the
[license](LICENSE.md) file. In the case that you need a different license, feel
free to contact me.

## Author
Daniel Lovasko (lovasko@freebsd.org)

