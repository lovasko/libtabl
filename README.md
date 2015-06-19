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
#include <stdlib.h>
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
  tabl_add_column(&t, "Name", TABL_CONTENT_STRING, TABL_ALIGN_LEFT);

  m_list_init(&values);
  while ((pwd = getpwent()) != NULL) {
    m_list_clear(&values);
    m_list_append(&values, M_LIST_COPY_DEEP, &pwd->pw_uid, sizeof(uid_t));
    m_list_append(&values, M_LIST_COPY_DEEP, pwd->pw_name, strlen(pwd->pw_name)+1);
    tabl_add_row(&t, &values);
  }
  endpwent();

  tabl_render(&t);
  return EXIT_SUCCESS;
}
```

Compile & run:
```
$ clang -o passwd passwd.c -ltabl
$ ./passwd
  UID Name
    0 root
    8 news
    9 man
   22 sshd
   25 smmsp
   26 mailnull
   68 pop
   78 auditdistd
   80 www
  845 hast
65534 nobody
```

## Documentation 
The usage of `libtabl` is completely linear. The whole workflow has four steps:
initialisation, column creation, row insertion and rendering of the table. The
order of tasks must be preserved.

### Initialisation
Use the `tabl_init(struct tabl* t)` function to initialise a already allocated
table.

### Column creation
Each column is specified by the `name` displayed in the header, the `content`
of the data in the column and the `align`ment of the data. To append a new
column to the table, use the `tabl_add_column(struct tabl* t, const char* name,
uint8_t content, uint8_t align)`. Appending a new column after already
appending some rows will result in an runtime error.

#### Content
Content can be one of the following:
 * `TABL_CONTENT_STRING` which expect the data to be a `NULL`-terminated string
 * `TABL_CONTENT_DECIMAL` which expect a single `int` of data that will be printed in the decimal format

#### Alignment
Alignment can be one of the following:
 * `TABL_ALIGN_LEFT` align content to the left
 * `TABL_ALIGN_RIGHT` align content to the right 

### Row insertion
Each row is represented as a separate `m_list`. As a security measure, the
number of columns and number of elements in the row is compared, to ensure the
arity consistence. To append a row, use the `tabl_add_row(struct tabl* t,
struct m_list* row)` function.

### Table rendering
To render the finished table onto the `stdout`, use the `tabl_render(struct
tabl* t)` function.

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

