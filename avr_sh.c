/***************************************************************************//**
  @file         avr_sh.c
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ARGS 64

// int avr_sh_parseLine(char * line, int line_size, char ** args)
// {

// }

/**
 * @brief Reads user input
 * @param line Char pointer for the line
 * @param line_size Size of the line
 * @return None
 */
void avr_sh_readLine(char * line, size_t line_size)
{
    if(line==NULL)
    {
        fprintf(stderr, "avr_sh_readLine: line is NULL in %s:%d\n", __FILE__, __LINE__);
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
    
    printf("Input Line: %s\n", line);
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

    int status = 0; // Status of input

    do
    {
        // Display prompt in Cyan color
        printf("\033[36mavr_sh$> \033[0m");
        
        // Read line
        char lineBuf[MAX_LINE_LENGTH];
        memset(lineBuf, 0, MAX_LINE_LENGTH);
        avr_sh_readLine(lineBuf, (size_t)MAX_LINE_LENGTH);

        // Parse Args
        char * argsBuf[MAX_ARGS];
        memset(argsBuf, 0, MAX_ARGS);
        // int numArgs = avr_sh_parseLine(lineBuf, strlen(lineBuf), argsBuf);
    
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
    printf("Starting AVR Shell!\n");
    if(argc != 0)
    {
        printf("AVR Shell Arguments:\n");
        for(int i=0; i < argc; i++)
        {
            printf("%s\n", argv[i]);
        }
    }
    
    // Load config files, if any.


    // Run command loop.
    avr_sh_loop();

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
