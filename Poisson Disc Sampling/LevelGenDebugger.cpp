/**
 * @brief: Contains the LevelGenDebugger class function implementations.
 * @file LevelGenDebugger.cpp
 * @author Gijs Sickenga
 * @date 17-04-2018
 */

#include "LevelGenDebugger.h"

// How fast the camera zooms in and out.
#define CAMERA_ZOOM_SPEED 300
// Whether to print the grid to the console.
#define PRINT_GRID_TO_CONSOLE true

/**
 * @brief: Initializes level gen debugging settings.
 * 
 * @param camera: A pointer to the camera.
 */
LevelGenDebugger::LevelGenDebugger(ICameraSceneNode *cameraSceneNode)
{
    camera = cameraSceneNode;

    // Whether to print the grid to the console.
    if (PRINT_GRID_TO_CONSOLE)
    {
        // Reverse height so grid doesn't print upside down.
        for (int y = Grid::GetInstance()->GetGridDimensions().Height - 1; y >= 0; y--)
        {
            for (int x = 0; x < Grid::GetInstance()->GetGridDimensions().Width; x++)
            {
                GridNode *n = Grid::GetInstance()->GetGridNodeAtCoordinate(vector2di(x, y));
                // Check if there's an object on this node.
                if (n->HasLevelObject())
                {
                    std::cout << 1;
                }
                else
                {
                    std::cout << 0;
                }
            }
            std::cout << std::endl;
        }
    }
}

/**
 * @brief: Update debugging behaviour.
 */
void LevelGenDebugger::Update()
{
    // Camera zooming with numpad 4 and 6.
    if (InputHandler::GetInstance()->IsKeyDown(irr::KEY_NUMPAD4))
    {
        // Move camera down on y axis.
        camera->setPosition(camera->getPosition() + vector3df(0, -CAMERA_ZOOM_SPEED * GameTime::GetDeltaTime(), 0));
    }
    if (InputHandler::GetInstance()->IsKeyDown(irr::KEY_NUMPAD6))
    {
        // Move camera up on y axis.
        camera->setPosition(camera->getPosition() + vector3df(0, CAMERA_ZOOM_SPEED * GameTime::GetDeltaTime(), 0));
    }

    // Regenerate level when G is pressed.
    if (InputHandler::GetInstance()->IsKeyPressed(irr::KEY_KEY_G))
    {
        // Delete existing level.
        LevelGenerator::CleanupLevel();

        // Generate a new level.
        LevelGenerator::GenerateLevel();
    }
}
