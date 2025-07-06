/***************************************************************************//**
  @file         avr_sh.c
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void avr_sh_loop(void)
{
    // Display a prompt
    // Read input
    // Parse input
    // Exec

    int status; // Status of input

    do
    {
        printf("avr-sh> ");
    
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
  // Load config files, if any.

  // Run command loop.
  avr_sh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}