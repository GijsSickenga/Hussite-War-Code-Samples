/**
 * @brief: Contains the GameTime struct function implementations.
 * @file GameTime.cpp
 * @author Gijs Sickenga
 * @date 20-03-2018
 */

#include "Application.h"
#include "GameTime.h"

// Define static variables first to prevent undefined ref errors.
unsigned int GameTime::_previousFrameTime = 0;
unsigned int GameTime::_currentFrameTime = 0;
float GameTime::_deltaTime = 0;

/**
 * @brief: Recalculates delta time for the current frame.
 * Should only be called in Application.cpp, at the start of a new frame.
 */
void GameTime::RecalculateDeltaTime()
{
    // Get the current frametime.
    _currentFrameTime = ApplicationInfo::device->getTimer()->getTime();

    // Calculate the elapsed time in seconds since the previous frame.
    _deltaTime = (_currentFrameTime - _previousFrameTime) / 1000.f;

    // Save the current frame time for our calculations next frame.
    _previousFrameTime = _currentFrameTime;
}

/**
 * @brief: Returns the elapsed time in seconds since the previous frame.
 * 
 * @return const float: The elapsed time in seconds since the previous frame.
 */
const float GameTime::GetDeltaTime()
{
    return _deltaTime;
}
