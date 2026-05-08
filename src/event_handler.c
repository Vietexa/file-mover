#include "include/event_handler.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>

enum FileType{
    FILE_UNKNOWN,
    FILE_IMAGE,
    FILE_VIDEO,
    FILE_AUDIO,
    FILE_PDF
};

static int is_temporary_file(const char *filename) {
    char* extension = strrchr(filename, '.');
    if (extension == NULL) return 0;

 return !strcmp(extension, ".crdownload")||
        !strcmp(extension, ".part")||
        !strcmp(extension, ".tmp")||
        !strcmp(extension, ".temp");
}

int handle_event(struct inotify_event *event, magic_t magic, const char *downloads_path, const char *home){

    if (!event->len) return 0;

    if (!(event->mask & IN_CLOSE_WRITE) && !(event->mask & IN_MOVED_TO)) return 0;
            
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

    enum FileType file_type = FILE_UNKNOWN;
                
    if (strncmp(mime, "image/", 6) == 0) {
        file_type = FILE_IMAGE; 
    }  

    else if (strncmp(mime, "video/", 6) == 0) {
        file_type = FILE_VIDEO; 
    }

    else if (strncmp(mime, "audio/", 6) == 0) {
        file_type = FILE_AUDIO;
    }

    else if (strcmp(mime, "application/pdf") == 0) {
        file_type = FILE_PDF;
    }

    char move_to_path[PATH_MAX];

    switch (file_type){
                
    case FILE_UNKNOWN: 
    return 0;

    case FILE_AUDIO:
    snprintf(move_to_path, sizeof(move_to_path), "%s/Music/%s", home, event->name);
    break;

    case FILE_IMAGE:
    snprintf(move_to_path, sizeof(move_to_path), "%s/Pictures/%s", home, event->name);
    break;

    case FILE_PDF:
    snprintf(move_to_path, sizeof(move_to_path), "%s/Documents/%s", home, event->name);
    break;

    case FILE_VIDEO:
    snprintf(move_to_path, sizeof(move_to_path), "%s/Videos/%s", home, event->name);
    break;

    default: return 0;

    }
                        
    if (rename(fullpath, move_to_path) != 0){
        perror("rename");
        return 1;
    }


return 0;
}