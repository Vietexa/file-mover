#ifndef UTILS_H
#define UTILS_H
#include "cjson/cJSON.h"


cJSON *parse_json_file(const char *path);
void copy_json_string(cJSON *root, const char *key, char *dst, size_t dst_size);

#endif // UTILS_H


