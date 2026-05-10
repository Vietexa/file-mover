#include "include/event_handler.h"

#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>

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

static void move_file_to_dir(const struct inotify_event *event, const char* home, const char *file_directory, const char *fullpath){

char move_to_path[PATH_MAX];
char candidate_name[NAME_MAX];

const char *dot = strrchr(event->name, '.');

if (dot && dot != event->name) {
    
    size_t base_len = dot - event->name;
    char base[NAME_MAX];
    char ext[NAME_MAX];

    snprintf(base, sizeof(base), "%.*s", (int)base_len, event->name);
    snprintf(ext, sizeof(ext), "%s", dot);

    int counter = 0;

    do {
        if (counter == 0) {
            snprintf(candidate_name, sizeof(candidate_name),
                     "%s%s", base, ext);
        } else {
            snprintf(candidate_name, sizeof(candidate_name),
                     "%s(%d)%s", base, counter, ext);
        }

        snprintf(move_to_path, sizeof(move_to_path),
                 "%s/%s/%s", home, file_directory, candidate_name);

        counter++;
    } while (access(move_to_path, F_OK) == 0);

} else {
    int counter = 0;

    do {
        if (counter == 0) {
            snprintf(candidate_name, sizeof(candidate_name),
                     "%s", event->name);
        } else {
            snprintf(candidate_name, sizeof(candidate_name),
                     "%s(%d)", event->name, counter);
        }

        snprintf(move_to_path, sizeof(move_to_path),
                 "%s/%s/%s", home, file_directory, candidate_name);

        counter++;
    } while (access(move_to_path, F_OK) == 0);
}

if (rename(fullpath, move_to_path) != 0) {
    perror("rename");
}



}

int handle_event(const struct inotify_event *event, magic_t magic, const char *downloads_path, const char *home){

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


    char *file_directory = NULL;

    switch (file_type){
                
    case FILE_UNKNOWN: 
    return 0;

    case FILE_AUDIO:
    file_directory = "Music";
    break;

    case FILE_IMAGE:
    file_directory = "Pictures";
    break;

    case FILE_PDF:
    file_directory = "Documents";
    break;

    case FILE_VIDEO:
    file_directory = "Videos";
    break;

    default: return 0;

    }

    move_file_to_dir(event, home, file_directory, fullpath);


    return 0;
}