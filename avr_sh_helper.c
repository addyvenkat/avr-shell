/***************************************************************************//**
  @file         avr_sh_helper.c
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include "avr_sh_helper.h"

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
