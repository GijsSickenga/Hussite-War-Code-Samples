/**
 * @brief: Contains the LevelGenDebugger class header information.
 * @file LevelGenDebugger.h
 * @author Gijs Sickenga
 * @date 20-04-2018
 */

#pragma once

#include <vector>

#include "IUpdatable.h"
#include "InputHandler.h"
#include "GameTime.h"

#include "TreeObject.h"

#include "PoissonDisc.h"
#include "LevelGenerator.h"

using irr::scene::ICameraSceneNode;

class LevelGenDebugger : public IUpdatable, public Singleton<LevelGenDebugger>
{
public:
    /**
     * @brief: Initializes level gen debugging settings.
     * 
     * @param camera: A pointer to the camera.
     */
    LevelGenDebugger(ICameraSceneNode *camera);
    /**
     * @brief: Update debugging behaviour.
     */
    virtual void Update() override;

private:
    /**
     * @brief: A pointer to the camera.
     * We need this in order to zoom the camera.
     */
    ICameraSceneNode *camera;
};
