/**
 * @brief: Contains the ButtonShortcuts struct function implementations.
 * @file ButtonShortcuts.cpp
 * @author Gijs Sickenga
 * @date 07-06-2018
 */

#include "ButtonShortcuts.h"

/**
 * @brief: Checks a number of buttons for shortcut events tied to those buttons.
 */
void ButtonShortcuts::UpdateShortcuts()
{
    // Building placement shortcuts.
    if (InputHandler::GetInstance()->IsKeyPressed(irr::KEY_KEY_B))
    {
        BuildingPlacer::GetInstance()->StartPlacingBuilding(
            BuildingPlacer::BuildingType::Barracks);
    }

    if (InputHandler::GetInstance()->IsKeyPressed(irr::KEY_KEY_F))
    {
        BuildingPlacer::GetInstance()->StartPlacingBuilding(
            BuildingPlacer::BuildingType::Farm);
    }

    if (InputHandler::GetInstance()->IsKeyPressed(irr::KEY_KEY_T))
    {
        BuildingPlacer::GetInstance()->StartPlacingBuilding(
            BuildingPlacer::BuildingType::TownCenter);
    }
}
