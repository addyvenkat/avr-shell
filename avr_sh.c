/***************************************************************************//**
  @file         avr_sh.c
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

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

/**
 * @brief Executes command
 * @param argc argument count
 * @param argv Array of args
 * @return None
 */
void avr_sh_cd(int argc, char ** argv)
{
    if (argc > 2)
    {
        fprintf(stderr, "%savr_sh_cd: \"cd\" expects at most 1 argument\n%s",
                BOLD_RED, DEFAULT);
        return;
    }

    const char *dest = NULL;
    char *dest_owned = NULL;   // malloc'd when we need to build a path
    char *oldcwd = NULL;
    char *newcwd = NULL;

    if (argc == 1) 
    {
        // cd -> $HOME (fallback to "/")
        const char *home = getenv("HOME");
        dest = (home && *home) ? home : "/";
        DEBUG_PRINT("cd to HOME (%s)\n", dest);
    } 
    else if (strcmp(argv[1], "-") == 0)
    {
        // cd - -> $OLDPWD and print it on success
        const char *oldpwd = getenv("OLDPWD");
        if (!oldpwd || !*oldpwd)
        {
            fprintf(stderr, "%savr_sh_cd: OLDPWD not set\n%s", BOLD_RED, DEFAULT);
            return;
        }
        dest = oldpwd;
        DEBUG_PRINT("cd to OLDPWD (%s)\n", dest);
    }
    else if (argv[1][0] == '~')
    {
        // cd ~ or cd ~/sub/path
        const char *home = getenv("HOME");
        if (!home || !*home)
        {
            fprintf(stderr, "%savr_sh_cd: HOME not set\n%s", BOLD_RED, DEFAULT);
            return;
        }
        if (argv[1][1] == '\0') 
        {
            dest = home; // just "~"
            DEBUG_PRINT("cd to HOME (%s)\n", dest);
        } 
        else if (argv[1][1] == '/')
        {
            // "~/<rest>"
            size_t need = strlen(home) + strlen(argv[1] + 1) + 1;
            dest_owned = (char *)malloc(need);
            if (!dest_owned) {
                fprintf(stderr, "%savr_sh_cd: out of memory\n%s", BOLD_RED, DEFAULT);
                return;
            }
            strcpy(dest_owned, home);
            strcat(dest_owned, argv[1] + 1); // appends "/<rest>"
            dest_owned[need-1] = '\0';
            dest = dest_owned;
            DEBUG_PRINT("cd to PATH (%s)\n", dest);
        }
        else
        {
            // "~<xyz>" not implemented in this minimal shell
            fprintf(stderr, "%savr_sh_cd: \"~<xyz>\" not supported\n%s", BOLD_RED, DEFAULT);
            return;
        }
    }
    else
    {
        // cd <path>
        dest = argv[1];
        DEBUG_PRINT("cd to PATH (%s)\n", dest);
    }

    // Save current cwd to update OLDPWD after a successful chdir
    oldcwd = getcwd(NULL, 0); // dynamic; may be NULL if cwd unreadable

    if (chdir(dest) != 0) {
        perror("cd");
        free(dest_owned);
        free(oldcwd);
        return;
    }

    newcwd = getcwd(NULL, 0);
    if (newcwd) {
        if (oldcwd) setenv("OLDPWD", oldcwd, 1);
        setenv("PWD", newcwd, 1);
    }

    // POSIX shells print the new dir for "cd -"
    if (argc == 2 && strcmp(argv[1], "-") == 0) {
        printf("%s\n", newcwd ? newcwd : dest);
    }
}

/**
 * @brief Executes command
 * @param argc argument count
 * @param argv Array of args
 * @return None
 */
void avr_sh_execute(int argc, char ** argv)
{
    if(strcmp(argv[0], "exit") == 0)
    {
        if(argc > 1)
        {
            fprintf(stderr, "%savr_sh_execute: \"exit\" command doesn't need additional args\n%s", BOLD_RED, DEFAULT);
            exit(EXIT_FAILURE); // Exit shell with failure
        }
        exit(EXIT_SUCCESS); // Exit the shell with success
    }
    else if(strcmp(argv[0], "help") == 0)
    {
        if(argc > 1)
        {
            fprintf(stderr, "%savr_sh_execute: \"help\" command doesn't need additional args %s:%d\n%s", BOLD_RED, __FILE__, __LINE__, DEFAULT);
        }
        else
        {
            printf("AVR SHELL:\n");
            printf("exit - Exiting the shell\n");
            printf("help - Lists supported commands\n");
            printf("cd - Change Directory\n");
        }
    }
    else if(strcmp(argv[0], "cd") == 0)
    {
        avr_sh_cd(argc, argv);
    }
    else
    {
        DEBUG_PRINT("%s():TODO\n", __func__);
    }
}

/**
 * @brief Parse user input into tokens
 * @param line Char pointer for the line
 * @param args array maintaining different token c-strings
 * @param maxArgs Maximum number of Arguments
 * @return number of arguments (tokens)
 */
int parseLine(char * line, char ** args, size_t maxArgs)
{
    size_t argCount = 0;

    // Skip leading spaces
    while (*line && isspace((unsigned char)*line)) line++;

    char * startPtr = line;
    char * spacePtr = line;
    while(startPtr && (argCount < maxArgs) )
    {
        startPtr = spacePtr;
        if(!startPtr)
        {
            break;
        }
        spacePtr = strchr(startPtr, ' ');
        if(spacePtr)
        {
            *spacePtr = '\0';
            spacePtr++;
        }
        if(startPtr)
        {
            args[argCount++] = startPtr;
        }
    }

    DEBUG_PRINT("Tokens (%zu): \n", argCount);
    for(int i = 0; i < (int)argCount; i++)
    {
        DEBUG_PRINT("%s\n", args[i]);
    }

    return (int)argCount;
}

/**
 * @brief Reads user input
 * @param line Char pointer for the line
 * @param line_size Size of the line
 * @return None
 */
void readLine(char * line, size_t line_size)
{
    if(line==NULL)
    {
        fprintf(stderr, "%sreadLine: line is NULL in %s:%d\n%s", BOLD_RED, __FILE__, __LINE__, DEFAULT);
        exit(EXIT_FAILURE);
    }
    
    // Read Input
    FILE * fp = stdin;
    if(fgets(line, line_size, fp) == NULL)
    {
        /* End-of-file */
        exit(EXIT_SUCCESS);
    }
    
    if(strcspn(line, "\n") < line_size)
    {
        line[strcspn(line,"\n")] = '\0'; // Remove newline and make line a C-string
    }
    else
    {
        line[line_size-1] = '\0'; // Input line, exceeded max characters, make it a C-string
    }
    
    DEBUG_PRINT("Input Line: %s\n", line);
}

/**
 * @brief Loop for getting input and executing the command
 * @param None
 * @return None
 */
void avr_sh_loop(void)
{
    // Parse input
    // Exec

    int status = 1; // Status of input

    do
    {
        // Display prompt in Cyan color
        printf("%savr_sh$> %s", CYAN, DEFAULT);
        
        // Read line
        char lineBuf[MAX_LINE_LENGTH];
        memset(lineBuf, 0, sizeof(lineBuf));
        readLine(lineBuf, (size_t)MAX_LINE_LENGTH);

        // Parse Args
        char * argsBuf[MAX_ARGS];
        memset(argsBuf, 0, sizeof(argsBuf));
        int numArgs = parseLine(lineBuf, argsBuf, (size_t)MAX_ARGS);
        
        if(numArgs == 0)
        {
            continue;
        }

        // Execute
        avr_sh_execute(numArgs, argsBuf);
    
    } while(status);
}

/**
   @brief Main entry function.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
    printf("%sStarting AVR Shell!%s\n", BOLD_MAGENTA, DEFAULT); // Bold-Magenta ANSI code
    if(argc != 0)
    {
        DEBUG_PRINT("%s(): AVR Shell Arguments:\n", __func__);
        for(int i=0; i < argc; i++)
        {
            DEBUG_PRINT("%s\n", argv[i]);
        }
    }
    
    // Load config files, if any.


    // Run command loop.
    avr_sh_loop();

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
