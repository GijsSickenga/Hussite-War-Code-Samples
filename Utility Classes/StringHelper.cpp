/**
 * @brief: Contains the StringHelper class function implementations.
 * @file StringHelper.cpp
 * @author Gijs Sickenga
 * @date 30-05-2018
 */

#include "StringHelper.h"

/**
 * @brief Returns a formatted string. Max length of final string is 255 characters.
 * 
 * @param baseString The base string to use for formatting.
 * @param stringArguments String formatting arguments.
 * @return std::string The formatted string.
 */
std::string StringHelper::Format(const char *const formatString, ...)
{
    // Start traversal of arguments.
    va_list args;
    va_start(args, formatString);

    // Allocate a buffer to store the string in.
    char buffer[256];
    // Apply string formatting.
    vsnprintf(buffer, sizeof(buffer), formatString, args);
    // Store the buffer in a string.
    std::string outputString = buffer;

    // End traversal of variadic arguments.
    va_end(args);
    // Return the formatted string.
    return outputString;
}
