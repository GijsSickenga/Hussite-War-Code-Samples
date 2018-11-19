/**
 * @brief: Contains the BuildQueue class header information.
 * @file BuildQueue.h
 * @author Gijs Sickenga
 * @date 01-06-2018
 */

#pragma once

#include <vector>
#include <functional>

#include <irrlicht.h>

#include <GameTime.h>
#include <IUpdatable.h>

class HeadsUpDisplay;

/**
 * @brief: Manages a build queue, represented internally by a vector list for
 * versatility. Enqueue an IQueueItem, which contains functions that manage how
 * it is handled by the BuildQueue.
 */
class BuildQueue : public IUpdatable
{
public:
    /**
     * @brief Constructs a BuildQueue.
     * Maximum queue capacity can be specified.
     * 
     * @param queueCapacity The maximum capacity of the queue.
     */
    BuildQueue(int queueCapacity);

    /**
     * @brief Delete all remaining queue items upon deletion of
     * the BuildQueue.
     */
    ~BuildQueue();

    /**
     * @brief Represents an item that can be enqueued in a BuildQueue.
     * Implement this in a class you want to enqueue on the BuildQueue.
     */
    class IQueueItem
    {
    public:
        virtual ~IQueueItem(){};

        /**
         * @brief How long the item takes to finish the queue when it is active.
         */
        virtual float GetQueueTime() = 0;

        /**
         * @brief The texture to show on the enqueued button of this item.
         */
        virtual irr::video::ITexture *GetBuildQueueButtonImage() = 0;

        /**
         * @brief Returns the index of the item in the queue + 1,
         * so it starts counting up from 1 at the start of the queue.
         */
        int GetQueueIndex()
        {
            return _queueIndex + 1;
        };

    protected:
        /**
         * @brief The function that verifies whether the item can be enqueued or not.
         * Use this to perform checks like spending resources for a unit being purchased.
         * Must return true for the item to be successfully enqueued.
         */
        virtual bool OnBuildQueueStart() = 0;

        /**
         * @brief Called when this item is cancelled in the build queue.
         */
        virtual void OnBuildQueueCancel() = 0;

    private:
        friend class BuildQueue;
        
        /**
         * @brief Holds the actual index of the item in the build queue.
         * 0 when it is the first item in the queue.
         */
        int _queueIndex;
        
        /**
         * @brief Called when the item finishes the queue, after which it is popped from it.
         */
        std::function<void()> OnBuildQueueFinish;

        /**
         * @brief Initializes the IQueueItem with an index and finish function.
         * 
         * @param index Index in the build queue.
         * @param finishFunction Function to call upon being popped from the build queue.
         */
        void Initialize(int index, std::function<void()> finishFunction)
        {
            _queueIndex = index;
            OnBuildQueueFinish = finishFunction;
        };
    };

    /**
     * @brief Attempt to enqueue the given item. If the item's OnBuildQueueStart
     * function evaluates to false, or the queue is full, this function will end
     * and return false. Returns true upon a successful enqueue.
     * 
     * @param item The item to enqueue.
     * @param finishFunction The function to call when the item finishes the queue.
     * @return bool Whether the item was successfully enqueued or not.
     */
    bool Enqueue(IQueueItem *item, std::function<void()> finishFunction);

    /**
     * @brief Cancels the specified item in the build queue.
     * 
     * @param item The item to cancel.
     * @return bool Whether the item was present and subsequently removed from the build queue.
     */
    bool Cancel(IQueueItem *item);

    /**
     * @brief Returns the progress of the current item in the queue.
     * 
     * @return float A number between 0 and 1 indicating the progress of the
     * current item in the queue.
     */
    float GetCurrentItemProgress();

    /**
     * @brief Returns a const pointer to the internal vector of enqueued items.
     * 
     * @return const std::vector<IQueueItem *>* const A const pointer to the build queue.
     */
    const std::vector<IQueueItem *>* const GetQueueList();

    /**
     * @brief: Updates the queue timer and handles dequeueing at the right time.
     */
    virtual void Update() override;

private:
    /**
     * @brief Max number of concurrent items in the build queue.
     */
    int _queueCapacity;

    /**
     * @brief Holds the timer value for the currently active item in the queue.
     */
    float _currentItemTimer;

    /**
     * @brief The internal vector. This is the "build queue" itself, essentially.
     */
    std::vector<IQueueItem *> _queue = {};

    /**
     * @brief Pops the current item from the build queue, and makes the second item
     * the first. Also calls the finish function on the popped item and resets the timer.
     */
    void PopCurrentItem();

    /**
     * @brief Decrements the indices of all items after the given starting index,
     * which should correspond to the item that is about to be deleted.
     * This is necessary for the queue indices to remain consistent for all items
     * in the queue.
     * 
     * Call this BEFORE removing an item, otherwise the first item will be skipped.
     * 
     * @param removedItemIndex The index of the item that is about to be removed.
     */
    void DecrementIndices(int removedItemIndex);
};
