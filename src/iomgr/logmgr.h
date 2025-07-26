#ifndef LOGMGR_H
#define LOGMGR_H

#include <stdio.h>

// Any log messages above the threshold will be suppressed
#define LOG_THRESHOLD 0

// Different log levels - just debug for now
#define LOG_DBG 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define Color_Red "\33[0:31m\\]" // Color Start
#define Color_end "\33[0m\\]" // To flush out prev settings

#define LOG_CMD(...) \
    do { \
        printf(ANSI_COLOR_CYAN); \
        printf(__VA_ARGS__); \
        printf(ANSI_COLOR_RESET); \
    } while (0)

#define LOG_CMD_L(level, ...) \
    do { \
        if ((level) <= LOG_THRESHOLD){ \
            LOG_CMD(__VA__ARGS__); \
        } \
    } while (0)

#define LOG_CTRL(...) \
    do { \
        printf(ANSI_COLOR_RED); \
        printf(__VA_ARGS__); \
        printf(ANSI_COLOR_RESET); \
    } while (0)

#define LOG_CTRL_L(level, ...) \
    do { \
        if ((level) <= LOG_THRESHOLD){ \
            LOG_CTRL(__VA_ARGS__); \
        } \
    } while (0)

#define LOG_IO(...) \
    do { \
        printf(ANSI_COLOR_YELLOW); \
        printf(__VA_ARGS__); \
        printf(ANSI_COLOR_RESET); \
    } while (0)

#define LOG_IO_L(level, ...) \
    do { \
        if ((level) <= LOG_THRESHOLD){ \
            LOG_IO(__VA_ARGS__); \
        } \
    } while (0)

#endif
