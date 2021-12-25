#include <stdio.h>
#include <stdlib.h>

enum open_status {
    OPEN_SUCCESS,
    OPEN_OTHER_ERROR,
    OPEN_FILE_PERM_DENIED,
    OPEN_FILE_NOT_EXIST
};

enum close_status {
    CLOSE_SUCCESS,
    CLOSE_OTHER_ERROR,
    CLOSE_FILE_NOT_EXIST
};

enum open_status file_open(FILE** file, const char* path, const char* user_rights);

enum close_status file_close(FILE** file);
