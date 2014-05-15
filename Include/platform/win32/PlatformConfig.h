#ifndef PLATFOEMCONFIG_INCLUDE
#define PLATFOEMCONFIG_INCLUDE
#include "PlatformDefine.h"

#ifdef FC_PLATFOEM
#undef FC_PLATFOEM
#endif

#define FC_PLATFOEM PLATFORM_WIN32

#define FC_UNUSED_PARAM(unusedparam) (void)unusedparam

#include "GL/glew.h"
#define ssize_t int

#define TOUCH_MAX_NUM 1

#endif