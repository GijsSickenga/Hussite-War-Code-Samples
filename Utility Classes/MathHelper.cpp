/**
 * @brief: CPP file for generic math functions class
 * 
 * @file MathHelper.cpp
 * @author Iantje
 * @date 2018-05-14
 */

#include "MathHelper.h"

/**
 * @brief: Interpolate between a and b by i, where i is between 0 and 1.
 * 0 returns a, 1 returns b, values in between are linearly interpolated.
 * 
 * @tparam T: Number type.
 * @param a: Value 1.
 * @param b: Value 2.
 * @param i: Interpolation amount.
 * @return float: The linearly interpolated value between a and b.
 */
float MathHelper::Lerp(float a, float b, float i)
{
    return a + (b - a) * i;
}

/**
 * @brief: Returns -1 for a negative number, 0 for 0 and 1 for a positive number.
 * 
 * @tparam T: Number type.
 * @param value: Value to check whether it is negative, 0, or positive for.
 * @return int: -1 for a negative number, 0 for 0 and 1 for a positive number.
 */
int MathHelper::Sign(float value)
{
    return (float(0) < value) - (value < float(0));
}
