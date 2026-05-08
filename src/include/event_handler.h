#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <magic.h>
#include <sys/inotify.h>

int handle_event(struct inotify_event *event, magic_t magic, const char *downloads_path, const char *home);

#endif //EVENT_HANDLER_H