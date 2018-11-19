/**
 * @brief: Contains the GameWorld class header information.
 * @file GameWorld.h
 * @author Gijs Sickenga
 * @date 19-03-2018
 */

#pragma once

#include <vector>
#include <algorithm>

#include <irrlicht.h>

#include "Singleton.h"

// Forward declare IUpdatable to prevent cyclic include.
class IUpdatable;

/**
 * @brief: Keeps track of all updatable objects and calls their update loops every frame.
 */
class GameWorld : public Singleton<GameWorld>
{
public:
    GameWorld();
    void Add(IUpdatable *object);
    bool Remove(IUpdatable *object);
    void UpdateAll();

private:
    /**
     * @brief: This list is used to add objects to the update list.
     * It prevents segmentation errors by storing objects temporarily,
     * and adding them to the update list at the end of the update loop.
     */
    std::vector<IUpdatable *> _addList;
    /**
     * @brief: The list of objects to call update on every frame.
     */
    std::vector<IUpdatable *> _updateList;
};
