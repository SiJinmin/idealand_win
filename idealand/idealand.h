#pragma once
#ifndef idealand_h
#define idealand_h

/// system defines, must be pre of includes for its usage
/* the system use utf-8 as unified encoding. */
#ifndef UNICODE
#define UNICODE
#endif
/* exclude a bunch of Windows header files when include windows.h */ 
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>



#ifdef _MSC_VER

#include <io.h>
#include <direct.h>
#include <tchar.h>
#include <Windows.h>
#include <wingdi.h>
#include <process.h>

#elif __GNUC__

#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/time.h>

#endif



#include "consts.h"
#include "structs.h"
#include "context.h"
#include "check.h"
#include "string.h"
#include "text.h"
#include "time.h"
#include "print.h"
#include "file.h"
#include "thread.h"

#endif
