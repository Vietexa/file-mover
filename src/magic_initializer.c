#include "include/magic_initializer.h"
#include <magic.h>
#include <stdio.h>
#include <unistd.h>

int initialize_magic(magic_t *magic_ptr){

   *magic_ptr = magic_open(MAGIC_MIME_TYPE);

    if (*magic_ptr == NULL) {
        fprintf(stderr, "magic_open failed\n");
        return 1;
    }

    if (magic_load(*magic_ptr, NULL) != 0) {
        fprintf(stderr, "magic_load failed: %s\n",
        magic_error(*magic_ptr));
        magic_close(*magic_ptr);
        return 1;
    }

    return 0;

}