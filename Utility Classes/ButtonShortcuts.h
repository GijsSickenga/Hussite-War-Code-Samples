/**
 * @brief: Contains the ButtonShortcuts struct header information.
 * @file ButtonShortcuts.h
 * @author Gijs Sickenga
 * @date 07-06-2018
 */

#pragma once

#include "IUpdatable.h"
#include "BuildingPlacer.h"

/**
 * @brief: Handles button shortcut events in its update function.
 */
struct ButtonShortcuts : public Singleton<ButtonShortcuts>
{
public:
    friend class Singleton;
    void UpdateShortcuts();
};
