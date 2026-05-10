#include "include/utils.h"

#include "cjson/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

cJSON *parse_json_file(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);

    char *buffer = malloc(len + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    size_t read_len = fread(buffer, 1, len, fp);
    buffer[read_len] = '\0';

    fclose(fp);

    cJSON *root = cJSON_Parse(buffer);
    free(buffer);

    return root;
}

void copy_json_string(cJSON *root, const char *key, char *dst, size_t dst_size)
{
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root, key);

    if (cJSON_IsString(item) && item->valuestring) {
        snprintf(dst, dst_size, "%s", item->valuestring);
        printf("The directory for %s is %s\n", key, item->valuestring);
    }
}