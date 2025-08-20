/***************************************************************************//**
  @file         avr_sh.c
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include "avr_sh_headers.h"
#include "avr_sh_builtins.h"
#include "avr_sh_helper.h"

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
            printf("cd - Change directory\n");
            printf("ls - List directory content\n");
            printf("pwd - Print current working directory");
        }
    }
    else if(strcmp(argv[0], "cd") == 0)
    {
        avr_sh_cd(argc, argv);
    }
    else if (strcmp(argv[0], "ls") == 0)
    {
        avr_sh_ls(argc, argv);
    }
    else if(strcmp(argv[0], "pwd") == 0)
    {
        avr_sh_pwd(argc);
    }
    else
    {
        DEBUG_PRINT("%s():TODO\n", __func__);
    }
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
