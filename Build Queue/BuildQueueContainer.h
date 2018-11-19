/**
 * @brief: Contains the BuildQueueContainer class header information.
 * @file BuildQueueContainer.h
 * @author Gijs Sickenga
 * @date 01-06-2018
 */

#pragma once

#include <BuildQueue.h>

/**
 * @brief Contains an internal build queue and a getter method to it.
 * Inherit this class in classes that need a build queue.
 */
class BuildQueueContainer
{
public:
    BuildQueueContainer(int queueCapacity) : _buildQueue(queueCapacity)
    {
        // Do nothing, just initialize build queue. ^
    };

    /**
     * @brief Returns the class's internal build queue.
     * 
     * @return BuildQueue The class's internal build queue.
     */
    BuildQueue &GetBuildQueue()
    {
        return _buildQueue;
    };

private:
    /**
     * @brief The class's internal build queue.
     */
    BuildQueue _buildQueue;
};
