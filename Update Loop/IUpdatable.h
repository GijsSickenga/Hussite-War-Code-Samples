/**
 * @brief: Contains the IUpdatable class header information.
 * @file IUpdatable.h
 * @author Gijs Sickenga
 * @date 29-03-2018
 */

#pragma once

#include "GameWorld.h"

/**
 * @brief: Interface that adds itself to the GameWorld update list upon creation.
 * Implementing this in your class means you have an Update function that gets called 
 * each frame.
 */
class IUpdatable
{
public:
    /**
     * @brief: The IUpdatable object adds itself to the GameWorld here.
     */
    IUpdatable();
    /**
     * @brief: The IUpdatable object removes itself from the GameWorld here.
     */
    virtual ~IUpdatable();

    /**
     * @brief: Called once per frame, so higher frame rates = more Update calls per second.
     * Must be overridden in subclass.
     * 
     * Remember to use Time::GetDeltaTime() for things that happen over time, like movement.
     */
    virtual void Update() = 0;
};
