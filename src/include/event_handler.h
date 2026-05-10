#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <linux/limits.h>
#include <magic.h>
#include <sys/inotify.h>

typedef struct Directories {
char downloads_dir[NAME_MAX];
char videos_dir[NAME_MAX];
char documents_dir[NAME_MAX];
char audio_dir[NAME_MAX];
char pictures_dir[NAME_MAX];
} Directories;

int handle_event(const struct inotify_event *event, magic_t magic, const char *downloads_path, const char *home, Directories *directories);

#endif //EVENT_HANDLER_H