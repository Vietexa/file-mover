#include "include/helper_downloads.h"
#include "include/inotify_initializer.h"
#include "include/magic_initializer.h"
#include "include/event_handler.h"

#include <errno.h>
#include <stdlib.h>

#include <sys/inotify.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <magic.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + NAME_MAX + 1))



int main() {

    int fd = 0;
    int wd = 0;
    magic_t magic = NULL;

    char downloads_path[1024];

    char *home_path = getenv("HOME");

    if (!home_path){
        fprintf(stderr, "Couldn't get the home path\n");
        return 1;

    }


    if (get_downloads_dir(downloads_path,sizeof(downloads_path)) != 0){
        return 1;
    }

    if(initialize_magic(&magic) != 0){
        return 1;
    }

    initialize_inotify(&fd,&wd, downloads_path);

    char buffer[BUF_LEN];

    while (1) {
        int length = read(fd, buffer, BUF_LEN);

        if (length < 0){
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            usleep(100000);
            continue;
        }
    perror("read");
    break;
    }

        int i = 0;

        while (i < length) {

            struct inotify_event *event = (struct inotify_event *)&buffer[i];

            if (handle_event(event, magic, downloads_path, home_path) != 0){
                break;
            }

            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    magic_close(magic);
    

    return 0;
}