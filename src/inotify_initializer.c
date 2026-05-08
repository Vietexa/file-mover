#include "include/inotify_initializer.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

void initialize_inotify(int *fd, int *wd, char *downloads_path){
 *fd = inotify_init();

    if (*fd < 0) {
        perror("inotify_init");
        exit(1);
    }

  *wd = inotify_add_watch(
        *fd,
        downloads_path,
        IN_CLOSE_WRITE | IN_MOVED_TO
    );

    if (*wd < 0) {
        perror("inotify_add_watch");
        close(*fd);
        exit(1);
    }

}