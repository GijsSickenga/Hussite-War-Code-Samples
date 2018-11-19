/**
 * @brief: Contains the IUpdatable class function implementations.
 * @file IUpdatable.cpp
 * @author Gijs Sickenga
 * @date 29-03-2018
 */

#include "IUpdatable.h"

/**
 * @brief: The IUpdatable object adds itself to the GameWorld here.
 */
IUpdatable::IUpdatable()
{
    // Add this object to the GameWorld update loop.
    GameWorld::GetInstance()->Add(this);
}

/**
 * @brief: The IUpdatable object removes itself from the GameWorld here.
 */
IUpdatable::~IUpdatable()
{
    // Remove this object from the GameWorld update loop.
    GameWorld::GetInstance()->Remove(this);
}
