#include "include/helper_downloads.h"
#include <stdio.h>
#include <string.h>

int get_downloads_dir(char *path, size_t buffer_size){
     FILE *fp = popen("xdg-user-dir DOWNLOAD", "r");
    
     if (!fp){
        fprintf(stderr, "Couldn't open xdg-user-dir, make sure it's installed\n");
        return 1;
    }

    if (!fgets(path, buffer_size, fp)) {
        fprintf(stderr, "Couldn't store the downloads path\n");
        pclose(fp);
         return 1;
    }
    

    path[strcspn(path, "\n")] = '\0';

    pclose(fp);
    return 0;
}