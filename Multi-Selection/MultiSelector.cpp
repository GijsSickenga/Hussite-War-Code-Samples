/**
 * @brief: Contains the MultiSelector class function implementations.
 * @file MultiSelector.cpp
 * @author Gijs Sickenga
 * @date 04-06-2018
 */

#include "MultiSelector.h"
#include "Selection.h"

const float MultiSelector::DefaultLineDimensions::HEIGHT = 0.2f;
const float MultiSelector::DefaultLineDimensions::THICKNESS = 2.f;

MultiSelector::MultiSelector(CameraController* cameraController)
{
    // Cache a reference to camera controller.
    _cameraController = cameraController;

    // Initialize cube scene nodes for selection area visualization lines.
    InitializeLine(_leftLine);
    InitializeLine(_rightLine);
    InitializeLine(_topLine);
    InitializeLine(_bottomLine);
}

/**
 * @brief Initializes a line.
 */
void MultiSelector::InitializeLine(ISceneNode *&line)
{
    // Add a cube scene node to visualize the line.
    line = ApplicationInfo::device->getSceneManager()->addCubeSceneNode(1);
    // Enable lighting so the line doesn't default to white.
    line->setMaterialFlag(EMF_LIGHTING, true);
    // For some reason, settings this to ECM_NONE prevents the line from
    // becoming pure white, so do this in order to give the line a custom color.
    line->getMaterial(0).ColorMaterial = irr::video::E_COLOR_MATERIAL::ECM_NONE;
    // Make ambient and diffuse color black so the line doesn't receive light.
    line->getMaterial(0).AmbientColor = {0};
    line->getMaterial(0).DiffuseColor = {0};
    // Make the line bright green.
    line->getMaterial(0).EmissiveColor = {255, 0, 255, 0};
    // Hide the line for now.
    line->setVisible(false);
}

/**
 * @brief Resizes the selection area to follow the mouse if it is visible.
 */
void MultiSelector::Update()
{
    // Make sure the visualization is visible, otherwise we don't need to resize it.
    if (_active)
    {
        // Update end point to match cursor position.
        vector2di mouseScreenPos = InputHandler::GetInstance()->GetMouseState().position;
        _endPoint = _cameraController->GetWorldPositionFromScreenPosition(mouseScreenPos);

        // Update selected area to match new end point.
        _selectedArea.SetArea(_startPoint, _endPoint);

        // Only make lines visible when selection area goes beyond a certain size.
        if (!_visible)
        {
            int minSize = Grid::GetInstance()->GetGridNodeSize() / 5.f;
            if (_selectedArea.bounds.Width > minSize || _selectedArea.bounds.Height > minSize)
            {
                // Make lines visible from this point onwards.
                _visible = true;
                ToggleVisualization();
            }
        }

        // Update selection visualization to new selected area.
        UpdateVisualization();
    }
}

/**
 * @brief Activates the selection area.
 */
void MultiSelector::Activate(vector3df startPoint)
{
    _active = true;

    // Set start point to given coordinate.
    SetStartPoint(startPoint);
}

/**
 * @brief Deactivates the selection area.
 */
void MultiSelector::Deactivate()
{
    _active = false;
    _visible = false;

    // Toggle line visibility.
    ToggleVisualization();
}

/**
 * @brief Stores all LevelObjects in the selection area that fit the criteria of
 * the specified filter function in the given list of objects. However, filtering
 * is only applied when there's more than 1 object in the selection area.
 * 
 * @param filterFunction Function to check the objects against. Only if this function
 * returns true will the object be added to the list of returned objects.
 * @param objects The list to add all found objects to.
 * @param mode The selection mode to perform on the found objects.
 */
void MultiSelector::GetObjectsInArea(std::function<bool(LevelObject *)> filterFunction,
                                     std::vector<LevelObject *> *&objects,
                                     SelectionMode mode)
{
    // Initialize values for grid nodes in square function call.
    vector2di bottomLeft = Grid::GetInstance()->GetCoordinateFromPosition(_selectedArea.bottomLeftCorner);

    vector3df endPos = vector3df(_selectedArea.bottomLeftCorner.X + _selectedArea.bounds.Width,
                                 0,
                                 _selectedArea.bottomLeftCorner.Z + _selectedArea.bounds.Height);

    vector2di endCoord = Grid::GetInstance()->GetCoordinateFromPosition(endPos);
    vector2di intBounds =
    {
        endCoord.X - bottomLeft.X,
        endCoord.Y - bottomLeft.Y
    };

    auto gridNodes = Grid::GetInstance()->GetGridNodesInSquare(bottomLeft, intBounds);

    // Hard check if exactly 1 tile selected and no objects passed in.
    if (gridNodes.size() == 1 && objects->size() == 0)
    {
        // 1 tile selected, check if it has an object on it.
        if (gridNodes.front()->HasLevelObject())
        {
            LevelObject *object = gridNodes.front()->GetLevelObject();
            // Perform selection on level object.
            PerformSelectionOnObject(objects, object, mode);
        }

        // Return, because we don't need to check for multiple tiles anymore.
        return;
    }

    // Loop over all grid nodes.
    for (GridNode *node : gridNodes)
    {
        if (node->HasLevelObject())
        {
            LevelObject *object = node->GetLevelObject();
            if (filterFunction(object))
            {
                // Perform selection on level object.
                PerformSelectionOnObject(objects, object, mode);
            }
        }
    }
}

/**
 * @brief Applies the specified selectionmode on the specified object.
 * 
 * @param objects List of objects to add or remove object from.
 * @param objectToPerformOn Object to perform selection on.
 * @param mode Selection mode to perform on object.
 */
void MultiSelector::PerformSelectionOnObject(std::vector<LevelObject *> *&objects,
                                             LevelObject *&objectToPerformOn,
                                             SelectionMode mode)
{
    // Handle all selection modes.
    switch (mode)
    {
        case SelectionMode::eAdd:
        {
            // Add object to selected objects.
            objects->push_back(objectToPerformOn);
            break;
        }
        case SelectionMode::eSubtract:
        {
            // (Attempt to) subtract object from selected objects.
            Selection::GetInstance()->DeselectObject(objectToPerformOn);
            break;
        }
    }
}

/**
 * @brief Sets the start point for the selection.
 * 
 * @param coordinate The new start point.
 */
void MultiSelector::SetStartPoint(vector3df coordinate)
{
    // Set start point.
    _startPoint = coordinate;
    // Also reset end point when setting a new start point.
    _endPoint = coordinate;
}

/**
 * @brief Toggles the visibility of the selection area visualization.
 */
void MultiSelector::ToggleVisualization()
{
    _leftLine->setVisible(_visible);
    _rightLine->setVisible(_visible);
    _topLine->setVisible(_visible);
    _bottomLine->setVisible(_visible);
}

/**
 * @brief Updates the selection visual based on the currently selected area.
 */
void MultiSelector::UpdateVisualization()
{
    vector3df center = _selectedArea.GetCenter();

    // Left line.
    _leftLine->setPosition
    (
        {
            _selectedArea.bottomLeftCorner.X,
            center.Y,
            center.Z
        }
    );
    _leftLine->setScale
    (
        {
            DefaultLineDimensions::THICKNESS,
            DefaultLineDimensions::HEIGHT,
            _selectedArea.bounds.Height + DefaultLineDimensions::THICKNESS
        }
    );

    // Right line.
    _rightLine->setPosition
    (
        {
            _selectedArea.bottomLeftCorner.X + _selectedArea.bounds.Width,
            center.Y,
            center.Z
        }
    );
    _rightLine->setScale
    (
        {
            DefaultLineDimensions::THICKNESS,
            DefaultLineDimensions::HEIGHT,
            _selectedArea.bounds.Height + DefaultLineDimensions::THICKNESS
        }
    );

    // Bottom line.
    _bottomLine->setPosition
    (
        {
            center.X,
            center.Y,
            _selectedArea.bottomLeftCorner.Z
        }
    );
    _bottomLine->setScale
    (
        {
            _selectedArea.bounds.Width + DefaultLineDimensions::THICKNESS,
            DefaultLineDimensions::HEIGHT,
            DefaultLineDimensions::THICKNESS
        }
    );

    // Top line.
    _topLine->setPosition
    (
        {
            center.X,
            center.Y,
            _selectedArea.bottomLeftCorner.Z + _selectedArea.bounds.Height
        }
    );
    _topLine->setScale
    (
        {
            _selectedArea.bounds.Width + DefaultLineDimensions::THICKNESS,
            DefaultLineDimensions::HEIGHT,
            DefaultLineDimensions::THICKNESS
        }
    );
}
