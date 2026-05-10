#include "include/helper_downloads.h"
#include "include/inotify_initializer.h"
#include "include/magic_initializer.h"
#include "include/event_handler.h"
#include "include/utils.h"

#include <errno.h>
#include <linux/limits.h>
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

    Directories directories = {0};

    cJSON *root = parse_json_file("config.json");

    if (!root){
        fprintf(stderr, "Could not parse the json file\n");
        return 1;
    }

    copy_json_string(root,"videos_dir",
        directories.videos_dir,
        sizeof(directories.videos_dir));

    copy_json_string(root,"audio_dir",
        directories.audio_dir,
        sizeof(directories.audio_dir));

    copy_json_string(root,"documents_dir",
        directories.documents_dir,
        sizeof(directories.documents_dir));

    copy_json_string(root,"pictures_dir",
        directories.pictures_dir,
        sizeof(directories.pictures_dir));

    cJSON_Delete(root);


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
            if (errno == EINTR)
            continue;

        perror("read");
        break;
    }

        int i = 0;

        while (i < length) {

            struct inotify_event *event = (struct inotify_event *)&buffer[i];

            if (handle_event(event, magic, downloads_path, home_path, &directories) != 0){
                goto cleanup;
            }

            i += EVENT_SIZE + event->len;
        }
    }

cleanup:

    inotify_rm_watch(fd, wd);
    close(fd);
    magic_close(magic);
    
    

    return 0;
}