/**
 * @brief: Contains the StringHelper class header information.
 * @file StringHelper.h
 * @author Gijs Sickenga
 * @date 30-05-2018
 */

#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

/**
 * @brief: Provides helper functions for strings.
 */
class StringHelper
{
public:
    /**
     * @brief Returns a formatted string.
     * 
     * @param formatString The base string to use for formatting.
     * @param ... The arguments to format the format string with.
     * @return std::string The formatted string.
     */
    static std::string Format(const char *const formatString, ...);
};
