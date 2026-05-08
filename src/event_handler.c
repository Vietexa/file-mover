#include "include/event_handler.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>

static int is_temporary_file(const char *filename) {
    return strstr(filename, ".crdownload") ||
           strstr(filename, ".part") ||
           strstr(filename, ".tmp") ||
           strstr(filename, ".temp");
}

int handle_event(struct inotify_event *event, magic_t magic, const char *downloads_path, const char *home){

    if (event->len) {

                if ((event->mask & IN_CLOSE_WRITE) || (event->mask & IN_MOVED_TO)) {
            
                char fullpath[PATH_MAX];

                snprintf(fullpath, sizeof(fullpath), "%s/%s", downloads_path, event->name);


                if (is_temporary_file(event->name)){
                    return 0;
                }

                const char *mime = magic_file(magic, fullpath);
                if (mime == NULL) {
                    fprintf(stderr, "magic_file failed for %s: %s\n",
                    fullpath, magic_error(magic));
                    return 1;
                }           
            
                printf("File: %s\n", event->name);
                printf("Full path: %s\n", fullpath);
                printf("MIME type: %s\n", mime);

                

                if (strncmp(mime, "image/", 6) == 0) {
                        printf("This is an image\n");

                        char move_to_path[PATH_MAX];
                        snprintf(move_to_path, sizeof(move_to_path), "%s/Pictures/%s", home, event->name);
                        if (rename(fullpath, move_to_path) != 0){
                            perror("rename");
                            return 1;
                        }
                        
                }  

                else if (strncmp(mime, "video/", 6) == 0) {
                        printf("This is a video\n");

                        char move_to_path[PATH_MAX];
                        snprintf(move_to_path, sizeof(move_to_path), "%s/Videos/%s", home, event->name);
                        if (rename(fullpath, move_to_path) != 0){
                            perror("rename");
                            return 1;
                        }
                    }

                else if (strncmp(mime, "audio/", 6) == 0) {
                    printf("This is audio\n");

                     char move_to_path[PATH_MAX];
                        snprintf(move_to_path, sizeof(move_to_path), "%s/Music/%s", home, event->name);
                        if (rename(fullpath, move_to_path) != 0){
                            perror("rename");
                            return 1;
                        }

                }

                else if (strcmp(mime, "application/pdf") == 0) {
                   char move_to_path[PATH_MAX];
                        snprintf(move_to_path, sizeof(move_to_path), "%s/Documents/%s", home, event->name);
                        if (rename(fullpath, move_to_path) != 0){
                            perror("rename");
                            return 1;
                        }
                }
                
            

            }

                
            }
return 0;
}