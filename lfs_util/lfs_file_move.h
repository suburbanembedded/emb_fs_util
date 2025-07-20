#pragma once

#include "lfs.h"

#ifdef __cplusplus
extern "C"
{
#endif

int lfs_file_movehandle(lfs_t* const fs, lfs_file_t* const old_file_t, lfs_file_t* const new_file_t);

#ifdef __cplusplus
}
#endif
