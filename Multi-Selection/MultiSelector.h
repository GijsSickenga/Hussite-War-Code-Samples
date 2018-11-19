/**
 * @brief: Contains the MultiSelector class header information.
 * @file MultiSelector.h
 * @author Gijs Sickenga
 * @date 04-06-2018
 */

#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include <irrlicht.h>

#include <IUpdatable.h>
#include <InputHandler.h>
#include <ApplicationInfo.h>
#include <CameraController.h>
#include <LevelObject.h>
#include <Grid.h>
#include <HeadsUpDisplay.h>

using irr::core::dimension2df;
using irr::core::vector3df;
using irr::core::vector2di;
using irr::scene::IMeshSceneNode;
using irr::video::EMF_LIGHTING;

class Selection;

/**
 * @brief: Manages 4 cube scene nodes that represent a selection area.
 * Has functions for getting the objects in the selected area.
 */
class MultiSelector : public IUpdatable
{
public:
    MultiSelector(CameraController* cameraController);

    /**
     * @brief Initializes a line.
     */
    void InitializeLine(ISceneNode *&line);

    // Determines whether to select or deselect given units.
    enum SelectionMode
    {
        eAdd,
        eSubtract
    };

    /**
     * @brief Contains the default values for visual line thickness and height.
     * Values defined at top of cpp.
     */
    struct DefaultLineDimensions
    {
        static const float HEIGHT;
        static const float THICKNESS;
    };

    /**
     * @brief Represents a selected area in world space coordinates.
     */
    struct SelectionArea
    {
        vector3df bottomLeftCorner = vector3df(0);
        dimension2df bounds = dimension2df(0, 0);

        void SetArea(vector3df startPoint, vector3df endPoint)
        {
            // Set X values.
            bottomLeftCorner.X = std::min(startPoint.X, endPoint.X);
            bounds.Width = std::abs(startPoint.X - endPoint.X);

            // Set Z values.
            bottomLeftCorner.Z = std::min(startPoint.Z, endPoint.Z);
            bounds.Height = std::abs(startPoint.Z - endPoint.Z);
        }

        vector3df GetCenter()
        {
            vector3df center =
            {
                bottomLeftCorner.X + (bounds.Width / 2.f),
                bottomLeftCorner.Y,
                bottomLeftCorner.Z + (bounds.Height / 2.f)
            };
            return center;
        }
    };

    /**
     * @brief Resizes the selection area to follow the mouse if it is visible.
     */
    virtual void Update() override;

    /**
     * @brief Activates the selection area.
     */
    void Activate(vector3df startPoint);

    /**
     * @brief Deactivates the selection area.
     */
    void Deactivate();

    /**
     * @brief Stores all LevelObjects in the selection area that fit the criteria of
     * the specified filter function in the given list of objects. However, filtering
     * is only applied when there's more than 1 object in the selection area.
     * 
     * @param filterFunction Function to check the objects against. Only if this function
     * returns true will the object be added to the list of objects.
     * @param objects The list to add all found objects to.
     * @param mode The selection mode to perform on the found objects.
     */
    void GetObjectsInArea(std::function<bool(LevelObject *)> filterFunction,
                          std::vector<LevelObject *> *&objects,
                          SelectionMode mode);

private:
    // Selection area visualization lines. These are stretched cube scene nodes.
    ISceneNode *_leftLine, *_rightLine, *_topLine, *_bottomLine;

    // The starting and ending points of the current selection area.
    vector3df _startPoint = vector3df(0);
    vector3df _endPoint = vector3df(0);

    // Whether the selection area is active.
    bool _active = false;

    // Whether the selection area visualization is visible.
    bool _visible = false;

    // The currently selected area.
    SelectionArea _selectedArea = SelectionArea();

    // Used to get mouse position in world space.
    CameraController* _cameraController = nullptr;
    
    /**
     * @brief Applies the specified selectionmode on the specified object.
     * 
     * @param objects List of objects to add or remove object from.
     * @param objectToPerformOn Object to perform selection on.
     * @param mode Selection mode to perform on object.
     */
    void PerformSelectionOnObject(std::vector<LevelObject *> *&objects,
                                  LevelObject *&objectToPerformOn,
                                  SelectionMode mode);

    /**
     * @brief Sets the start point for the selection.
     * 
     * @param coordinate The new start point.
     */
    void SetStartPoint(vector3df coordinate);

    /**
     * @brief Toggles the visibility of the selection area visualization.
     */
    void ToggleVisualization();

    /**
     * @brief Updates the visual square based on the currently selected area.
     */
    void UpdateVisualization();
};
