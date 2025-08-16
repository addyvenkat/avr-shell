/***************************************************************************//**
  @file         avr_sh_helper.h
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include "avr_sh_headers.h"

/**
 * @brief Parse user input into tokens
 * @param line Char pointer for the line
 * @param args array maintaining different token c-strings
 * @param maxArgs Maximum number of Arguments
 * @return number of arguments (tokens)
 */
int parseLine(char * line, char ** args, size_t maxArgs);

/**
 * @brief Reads user input
 * @param line Char pointer for the line
 * @param line_size Size of the line
 * @return None
 */
void readLine(char * line, size_t line_size);
