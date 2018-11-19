/**
 * @brief: Contains the BuildQueue class function implementations.
 * @file BuildQueue.cpp
 * @author Gijs Sickenga
 * @date 01-06-2018
 */

#include "BuildQueue.h"
#include "HeadsUpDisplay.h"

/**
 * @brief Constructs a BuildQueue.
 * Maximum queue capacity can be specified.
 * 
 * @param queueCapacity The maximum capacity of the queue.
 */
BuildQueue::BuildQueue(int queueCapacity)
{
    _queueCapacity = queueCapacity;
}

/**
 * @brief Delete all remaining queue items upon deletion of
 * the BuildQueue.
 */
BuildQueue::~BuildQueue()
{
    // Loop over all remaining items in the queue.
    for (IQueueItem * item : _queue)
    {
        delete item;
    }
}

/**
 * @brief Attempt to enqueue the given item. If the item's OnBuildQueueStart
 * function evaluates to false, or the queue is full, this function will end
 * and return false. Returns true upon a successful enqueue.
 * 
 * @param item The item to enqueue.
 * @param finishFunction The function to call when the item finishes the queue.
 * @return bool Whether the item was successfully enqueued or not.
 */
bool BuildQueue::Enqueue(IQueueItem *item, std::function<void()> finishFunction)
{
    // Make sure there is still space left in the build queue.
    if (_queue.size() + 1 > _queueCapacity)
    {
        // Build queue capacity reached, so return false.
        return false;
    }

    // Make sure the requirements for the enqueue are met.
    if (!item->OnBuildQueueStart())
    {
        // Requirements not met, so return false.
        return false;
    }

    // Initialize item before pushing it to the queue.
    item->Initialize(_queue.size(), finishFunction);
    _queue.push_back(item);

    // Refresh queue UI.
    HeadsUpDisplay::GetInstance()->UpdateBuildQueueOrder(this);

    // Return true, since the item was successfully enqueued.
    return true;
}

/**
 * @brief Cancels the specified item in the build queue.
 * 
 * @param item The item to cancel.
 * @return bool Whether the item was present and subsequently removed from the build queue.
 */
bool BuildQueue::Cancel(IQueueItem *item)
{
    // Search for the specified item in the build queue.
    auto i = std::find(_queue.begin(), _queue.end(), item);

    // Check if we found the specified item in the build queue.
    if (i != _queue.end())
    {
        // Check if the current item was cancelled.
        if (i == _queue.begin())
        {
            // Reset timer, since the current item was cancelled,
            // and we don't want the progress on it to roll over to the next item.
            _currentItemTimer = 0;
        }

        // Call cancel function on the item before erasing it from the queue.
        (*i)->OnBuildQueueCancel();

        // Decrement indices on all items after the erased item.
        DecrementIndices((*i)->_queueIndex);

        // Delete the item.
        delete (*i);

        // Erase the item from the build queue.
        _queue.erase(i);

        // Refresh queue UI.
        HeadsUpDisplay::GetInstance()->UpdateBuildQueueOrder(this);

        // Return true, since the item was found in the build queue.
        return true;
    }
    else
    {
        // The specified item was not present in the build queue,
        // so return false.
        return false;
    }
}

/**
 * @brief Returns the progress of the current item in the queue.
 * 
 * @return float A number between 0 and 1 indicating the progress of the
 * current item in the queue.
 */
float BuildQueue::GetCurrentItemProgress()
{
    // Return 1 if there are no items present in the queue.
    if (_queue.size() == 0)
    {
        return 1;
    }

    // Return progress of current item.
    return irr::core::clamp(_currentItemTimer / _queue.front()->GetQueueTime(), 0.f, 1.f);
}

/**
 * @brief Returns a const pointer to the internal vector of enqueued items.
 * 
 * @return const std::vector<IQueueItem *>* const A const pointer to the build queue.
 */
const std::vector<BuildQueue::IQueueItem *>* const BuildQueue::GetQueueList()
{
    return &_queue;
}

/**
 * @brief: Updates the queue timer and handles dequeueing at the right time.
 */
void BuildQueue::Update()
{
    // If the queue is empty, return.
    if (_queue.size() == 0)
    {
        return;
    }

    // Increment timer for the current item.
    _currentItemTimer += GameTime::GetDeltaTime();

    // Update queue progress bar.
    HeadsUpDisplay::GetInstance()->UpdateBuildQueueProgressBar(this);

    // Check if the timer for the current item has finished.
    if (_currentItemTimer >= _queue.front()->GetQueueTime())
    {
        PopCurrentItem();
        _currentItemTimer = 0;
    }
}

/**
 * @brief Pops the current item from the build queue, and makes the second item
 * the first. Also calls the finish function on the popped item and resets the timer.
 */
void BuildQueue::PopCurrentItem()
{
    // Call finish function on the current item before popping it from the queue.
    _queue.front()->OnBuildQueueFinish();

    // Decrement the queue indices for all enqueued items, because they have
    // all been moved ahead by 1 spot.
    DecrementIndices(0);

    // Delete the current item.
    delete (_queue.front());

    // Erase the current item.
    _queue.erase(_queue.begin());

    // Refresh queue UI.
    HeadsUpDisplay::GetInstance()->UpdateBuildQueueOrder(this);
}

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
void BuildQueue::DecrementIndices(int removedItemIndex)
{
    // Loop over all items after the given index.
    for (int i = removedItemIndex + 1; i < _queue.size(); i++)
    {
        // Decrement queue index.
        _queue.at(i)->_queueIndex--;
    }
}
