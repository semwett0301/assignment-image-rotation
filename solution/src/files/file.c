#include "file.h"
#include <errno.h>

enum open_status file_open(FILE** file, const char* path, const char* user_rights) {
    *file = fopen(path, user_rights);
    if (*file) {
        return OPEN_SUCCESS;
    } else if (errno == ENOENT){
        return OPEN_FILE_NOT_EXIST;
    } else if (errno == EACCES) {
        return OPEN_FILE_PERM_DENIED;
    }

    return OPEN_OTHER_ERROR;
}

enum close_status file_close(FILE** file) {
    if (*file) {
        if (fclose(*file) == 0) {
//            free(*file);
            return CLOSE_SUCCESS;
        } else {
            return CLOSE_OTHER_ERROR;
        }
    }

    return CLOSE_FILE_NOT_EXIST;
}
