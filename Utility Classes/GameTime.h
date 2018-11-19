/**
 * @brief: Contains the GameTime struct header information.
 * @file GameTime.h
 * @author Gijs Sickenga
 * @date 20-03-2018
 */

#pragma once

class Application;

/**
 * @brief: Contains time related information that can be used in Update calls and such.
 */
struct GameTime
{
public:
    static const float GetDeltaTime();

private:
    // Befriend application so it can call RecalculateDeltaTime each frame.
    friend Application;
    static void RecalculateDeltaTime();
    static unsigned int _previousFrameTime;
    static unsigned int _currentFrameTime;
    static float _deltaTime;
};
