/***************************************************************************//**
  @file         avr_sh_headers.h
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ARGS 64

#ifdef DEBUG_PRINTS_ENABLED
#define DEBUG_PRINT(format, ...) \
    do { \
        printf("[DEBUG][%s:%d] " format, __FILE__, __LINE__, __VA_ARGS__); \
    }while(0)
#else
#define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif // DEBUG_PRINTS_ENABLED
    

#define BOLD_RED "\033[1;31m"
#define BOLD_MAGENTA "\033[1;35m"
#define CYAN "\033[36m"
#define DEFAULT "\033[0m"
