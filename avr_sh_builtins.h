/***************************************************************************//**
  @file         avr_sh_builtins.h
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include "avr_sh_headers.h"

/**
 * @brief "cd" - Change Directory
 * @param argc argument count
 * @param argv Array of args
 * @return None
 */
void avr_sh_cd(int argc, char ** argv);

/**
 * @brief "ls" - List Directory
 * @param argc argument count
 * @param argv Array of args
 * @return None
 */
void avr_sh_ls(int argc, char ** argv);

/**
 * @brief "pwd" - Print working directory
 * @param argc argument count
 * @return None
 */
void avr_sh_pwd(int argc);
