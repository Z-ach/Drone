#ifndef LOGMGR_H
#define LOGMGR_H

#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define Color_Red "\33[0:31m\\]" // Color Start
#define Color_end "\33[0m\\]" // To flush out prev settings

#define LOG_CMD(...) printf(ANSI_COLOR_CYAN); printf(__VA_ARGS__); printf(ANSI_COLOR_RESET);
#define LOG_CTRL(...) printf(ANSI_COLOR_RED); printf(__VA_ARGS__); printf(ANSI_COLOR_RESET);
#define LOG_IO(...) printf(ANSI_COLOR_YELLOW); printf(__VA_ARGS__); printf(ANSI_COLOR_RESET);

#endif
