/**
 * @brief: Contains the GameWorld class function implementations.
 * @file GameWorld.cpp
 * @author Gijs Sickenga
 * @date 19-03-2018
 */

#include "GameWorld.h"
#include "IUpdatable.h"

/**
 * @brief: Constructor for GameWorld.
 */
GameWorld::GameWorld()
{
}

/**
 * @brief: Adds the specified object to the add list,
 * from which it will be transferred to the update list at the
 * end of the update loop.
 * 
 * @param object: The object to add to the update list.
 */
void GameWorld::Add(IUpdatable *object)
{
    _addList.push_back(object);
}

/**
 * @brief: Sets the specified object as null, so it will be removed in
 * the update loop automatically.
 * 
 * @param object: The object to remove from the update list.
 * @return bool: Whether the object was found in the update list.
 */
bool GameWorld::Remove(IUpdatable *object)
{
    // Search for the specified object in the update list.
    std::vector<IUpdatable *>::iterator i = std::find(_updateList.begin(),
                                                      _updateList.end(),
                                                      object);

    // Check if we found the specified object in the update list.
    if (i != _updateList.end())
    {
        // Set the object in the iterator to null so it's removed
        // in the update loop automatically.
        (*i) = nullptr;

        // Return true, since the object was found in the update list.
        return true;
    }
    else
    {
        // Object not found in update list.
        // Search for it in add list instead, just in case someone tried destroying an
        // object that was added earlier on the same frame.
        std::vector<IUpdatable *>::iterator i = std::find(_addList.begin(),
                                                          _addList.end(),
                                                          object);

        // Check if we found the specified object in the add list.
        if (i != _addList.end())
        {
            // Remove object from add list, since someone wanted it deleted
            // on the same frame it was created.
            _addList.erase(i);

            // Return true, since the object was found in the add list.
            return true;
        }
        else
        {
            printf("Tried to remove an object from GameWorld that wasn't present in GameWorld (any longer).");

            // Return false, since the object wasn't found in either list.
            return false;
        }
    }
}

/**
 * @brief: Calls Update method on all objects in the update list.
 * Removes null objects in the list automatically.
 * Adds all objects in the add list to the update list after the update loop.
 */
void GameWorld::UpdateAll()
{
    // Iterate over all objects in the update list.
    for (std::vector<IUpdatable *>::iterator i = _updateList.begin();
         i != _updateList.end();)
    {
        // Make sure the object isn't null.
        if ((*i))
        {
            // Object not null, so call Update.
            (*i)->Update();
            // Increment iterator to move to the next object in the list.
            i++;
        }
        else
        {
            // Object was marked null, so we need to remove it from the list.
            // Erase the object from the list, and get a new iterator to continue the loop.
            i = _updateList.erase(i);
        }
    }

    // Iterate over all objects in the add list to add objects to the update list.
    // This is done at the end to prevent segmentation errors when adding inside update loop.
    for (std::vector<IUpdatable *>::iterator i = _addList.begin();
         i != _addList.end();
         i++)
    {
        // Add the current object to the update list.
        _updateList.push_back((*i));
    }

    // Clear the list after all objects have been added.
    _addList.clear();
}
