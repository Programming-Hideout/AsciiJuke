#pragma once

#include "filesystem.h"
#include "stdbool.h"
#include <dirent.h>
#include <errno.h>

bool dir_exists(const char *dirpath) {
    DIR *dir = opendir(dirpath);
    if (dir) {
        closedir(dir);
        return true;
    }
    // returns false even if other errors occurred
    return false;
}
