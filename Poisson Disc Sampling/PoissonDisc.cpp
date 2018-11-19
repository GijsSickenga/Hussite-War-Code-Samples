/**
 * @brief: Contains the PoissonDisc class function implementations.
 * @file PoissonDisc.cpp
 * @author Gijs Sickenga
 * @date 17-04-2018
 */

#include "PoissonDisc.h"

// The amount of new points generated for every processed point.
// Higher values lead to points being grouped closer together.
#define POINTS_PER_LOOP 30

/**
 * @brief: Generates a uniformly distributed list of 2D point coordinates,
 * with a specified minimum distance between points, on a grid with a given
 * set of dimensions.
 * 
 * @param minimumDistance: The minimum distance between sample points.
 * @param mapDimensions: The dimensions of the map to generate the points on.
 * @param excludedSections: Optional sections of the map to not generate points on.
 * @param existingPoints: Optional base list of points to start generating new
 * points around. Use this parameter if you want to generate a list of points that
 * sit between the points passed in with this parameter.
 * @param maxNumberOfPoints: Max number of points to generate. If this parameter
 * is passed in, points will only be generated around the first point in the
 * existingPoints list.
 * @return ListOfPoints: The uniformly distributed list of points.
 */
ListOfPoints PoissonDisc::GeneratePoints(int minimumDistance,
                                         dimension2du mapDimensions,
                                         const std::vector<MapSection> &excludedSections,
                                         const std::vector<vector2di> &existingPoints,
                                         const int maxNumberOfPoints)
{
    // ** INITIALIZATION ** //
    // The final list of evenly distributed points (vector2di values).
    ListOfPoints outputList = ListOfPoints();
    // The queue of generated points that still need to be processed (vector2di values).
    QueueOfPoints processQueue = QueueOfPoints();

    // The size to use for the point grid cells.
    // This value is calculated such, that the diagonal of a grid cell = minimum
    // distance between points.
    float pointGridCellSize = minimumDistance / sqrt(2);
    // Width and height of grid, in cell count.
    int pointGridWidth = ceil(mapDimensions.Width / pointGridCellSize);
    int pointGridHeight = ceil(mapDimensions.Height / pointGridCellSize);

    // The 2D grid of vector2di points, used to efficiently check for points in close proximity.
    PointGrid2D pointGrid;

    // Initialize the dimensions of the point grid.
    pointGrid.dimensions = dimension2du(pointGridWidth, pointGridHeight);

    // Initialize the point grid with empty rows.
    for (int i = 0; i < pointGridHeight; i++)
    {
        // Initialize point grid row with a vector2di list, with grid width as size.
        // Initialize all points in the row to default value (-1, -1) to be able to check
        // for unset tiles later. Chosen value is (-1, -1), because a point could never
        // normally be generated at this position, as it is outside the bounds of the point grid.
        pointGrid.grid.push_back(ListOfPoints(pointGridWidth, vector2di(-1, -1)));
    }
    
    // Check if any existing points were passed in.
    if(existingPoints.empty())
    {
        // No existing points passed in, so generate a first point at random.

        // ** GENERATE FIRST POINT ** //
        // Generate the first point close to the center, to avoid bugs at the edge.
        int firstPointWidthRange = CustomRandom::Range(mapDimensions.Width * 0.4f, mapDimensions.Width * 0.6f);
        int firstPointHeightRange = CustomRandom::Range(mapDimensions.Height * 0.4f, mapDimensions.Height * 0.6f);
        // Generate first point at random position.
        vector2di firstPoint = vector2di(firstPointWidthRange,
                                         firstPointHeightRange);

        // Add first point to process queue, output list and grid.
        processQueue.push(firstPoint);
        outputList.push_back(firstPoint);
        // Get grid tile corresponding to first point.
        vector2di gridTile = PointToGridTile(firstPoint, pointGridCellSize);
        // Add first point to its corresponding tile in the point grid.
        pointGrid.grid[gridTile.X][gridTile.Y] = firstPoint;
    }
    else
    {
        // Existing points passed in, so fill the processQueue and point grid
        // with them, but not the output list, since we don't want to include
        // these points in the final generated list.

        // This will ensure that points are generated around the points in the
        // existing points list, and that they will never be generated too close
        // to any points in this list.

        // Loop over all points in the list.
        for (vector2di point : existingPoints)
        {
            // Add point to process queue and grid.
            processQueue.push(point);
            // Get grid tile corresponding to new point.
            vector2di gridTile = PointToGridTile(point, pointGridCellSize);
            // Add new point to its corresponding tile in the point grid.
            pointGrid.grid[gridTile.X][gridTile.Y] = point;
        }
    }

    // Counter that's used if a max point amount was defined.
    int numberOfPoints = 0;

    // ** GENERATE ALL SAMPLE POINTS ** //
    // Generate the rest of the points by processing them all untill there's no
    // more points left to process, starting with the first generated point.
    while (!processQueue.empty() && numberOfPoints < maxNumberOfPoints)
    {
        // The point currently being processed.
        vector2di currentPoint = processQueue.front();

        // Generate a set number of points around the current point.
        for (int i = 0; i < POINTS_PER_LOOP; i++)
        {
            // Generate a randomly placed new point around current point.
            vector2di newPoint = GenerateRandomPointAroundPoint(currentPoint, minimumDistance);

            // Make sure the point is within the bounds of the map and not too close
            // to any previously generated points.
            if (InsideRectangle(newPoint, vector2di(0), mapDimensions) &&
                !InNeighborhood(newPoint, minimumDistance, pointGrid, pointGridCellSize))
            {
                // Point was inside map and not too close to any existing points.
                // Add new point to process queue, output list and grid.
                processQueue.push(newPoint);
                outputList.push_back(newPoint);
                // Get grid tile corresponding to new point.
                vector2di gridTile = PointToGridTile(newPoint, pointGridCellSize);
                // Add new point to its corresponding tile in the point grid.
                pointGrid.grid[gridTile.X][gridTile.Y] = newPoint;

                numberOfPoints++;
                if (maxNumberOfPoints < MAX_INT)
                {
                    break;
                }
            }
        }

        if (!maxNumberOfPoints < MAX_INT)
        {
            // Pop the processed point from the process queue.
            processQueue.pop();
        }
    }

    // For each excluded section, remove the points in that section from
    // the output list.
    for (MapSection section : excludedSections)
    {
        ExcludeSection(outputList, section);
    }

    // Return the final list of points.
    return outputList;
}

/**
 * @brief: Returns the grid tile the given point is on, on a grid with the
 * given cell size.
 * 
 * @param point: The point to get the corresponding grid tile for.
 * @param cellSize: The size of a grid cell.
 * @return vector2di: The x and y indices of the grid tile the point is on.
 */
vector2di PoissonDisc::PointToGridTile(vector2di point, float cellSize)
{
    // Derive tile coordinates from point coordinates and cell size.
    // Automatically rounded down with implicit int cast.
    int tileX = point.X / cellSize;
    int tileY = point.Y / cellSize;

    // Return tile indices as vector2di.
    return vector2di(tileX, tileY);
}

/**
 * @brief: Generates a random point around the given point, between the
 * given minimum distance and twice the minimum distance of the source point.
 * 
 * @param point: The point around which to generate the new point.
 * @param minimumDistance: The minimum distance from the source point to
 * generate the new point.
 * @return vector2di: The newly generated point.
 */
vector2di PoissonDisc::GenerateRandomPointAroundPoint(vector2di point, int minimumDistance)
{
    // Randomize distance from source point, between minimum distance and
    // 2 * minimum distance.
    float distanceFromCenter = minimumDistance * (1 + CustomRandom::Range(0.f, 1.f));
    // Generate a random angle in radians.
    float angle = 2 * M_PI * CustomRandom::Range(0.f, 1.f);

    // Generate new point coordinates from random angle and distance from
    // source point.
    int newPointX = point.X + distanceFromCenter * cos(angle);
    int newPointY = point.Y + distanceFromCenter * sin(angle);

    // Return new point coordinates as vector2di.
    return vector2di(newPointX, newPointY);
}

/**
 * @brief: Returns whether the given point is inside the given rectangle.
 * 
 * @param point: The point for which to check whether it is inside the rectangle.
 * @param offset: The top left corner offset of the rectangle.
 * @param dimensions: The dimensions of the rectangle.
 * @return bool: Whether the point is inside the rectangle.
 */
bool PoissonDisc::InsideRectangle(vector2di point,
                                  vector2di offset,
                                  dimension2du dimensions)
{
    return point.X >= offset.X && point.X < offset.X + dimensions.Width &&
           point.Y >= offset.Y && point.Y < offset.Y + dimensions.Height;
}

/**
 * @brief: Returns whether a point is already present on the grid that is
 * less than the minimum distance between points away from the given point.
 * 
 * @param centerPoint: The point to check for close neighbors for.
 * @param minimumDistance: The minimum distance between points.
 * @param pointGrid: The grid to check for neighboring points on.
 * @param cellSize: The cell size of the point grid.
 * @return bool: Whether there are any points within minimum distance or less
 * from the given point.
 */
bool PoissonDisc::InNeighborhood(vector2di centerPoint,
                                 int minimumDistance,
                                 PointGrid2D pointGrid,
                                 float cellSize)
{
    // Get the grid tile of the given point.
    vector2di centerTile = PointToGridTile(centerPoint, cellSize);

    // Get all points on grid tiles around the center tile,
    // including the point's own grid tile.
    ListOfPoints pointsAroundCenterTile = GetPointsAroundTile(centerTile, pointGrid);

    // Loop over all points around the center tile and determine proximity.
    for (vector2di otherPoint : pointsAroundCenterTile)
    {
        // Make sure there is a non default (-1, -1) point on the tile we sampled.
        if (otherPoint != vector2di(-1, -1))
        {
            // Check the distance of the neighboring point from the centerpoint.
            if (otherPoint.getDistanceFrom(centerPoint) < minimumDistance)
            {
                // Point is within minimum distance, so return true.
                return true;
            }
        }
    }

    // No point found within minimum distance, so return false.
    return false;
}

/**
 * @brief: Returns a list of points around the given tile, within the given
 * tile range.
 * 
 * @param centerTile: The tile to return a list of closely neighboring points for.
 * @param pointGrid: The point grid to check for points on.
 * @return ListOfPoints: The list of closely neighboring points.
 */
ListOfPoints PoissonDisc::GetPointsAroundTile(vector2di centerTile, PointGrid2D pointGrid)
{
    // The list to fill with neighboring points.
    ListOfPoints neighboringPoints;
    
    // The amount of tiles to check is 25 - 1, because we check a 5 by 5 grid around
    // the center tile, minus the top left and bottom right corners, because they are
    // always outside the minimum distance radius. This results in the loop starting
    // at index 1, and needing to be run once fewer, hence 25 - 1.
    int tilesToCheck = 24;
    // We're checking a 5 by 5 grid, so row length is 5.
    int rowLength = 5;
    // Use half row length as distance from center tile to edges of area to check.
    // Rounded down due to implicit int cast.
    int halfRowLength = rowLength / 2;

    // Loop through all tiles surrounding the given center tile (including the center tile).
    // We start at 1, because the top left corner is excluded.
    for (int tileNumber = 1; tileNumber < tilesToCheck; tileNumber++)
    {
        // Skip the corner tiles, since points on them can never be within
        // minimum distance. The first and last corners are never included
        // in the loop, so we don't have to check for those.
        if (tileNumber == 4 || tileNumber == 20)
        {
            // Skip the tile and continue the loop.
            continue;
        }

        // Calculate grid coordinates of current tile in loop.
        vector2di tile = {centerTile.X - halfRowLength + tileNumber % rowLength,
                          centerTile.Y - halfRowLength + tileNumber / rowLength};

        // Make sure the tile is within the bounds of the point grid, otherwise skip.
        if (!InsideRectangle(tile, vector2di(0), pointGrid.dimensions))
        {
            // Skip the tile and continue the loop.
            continue;
        }

        // Tile is within bounds of point grid, so grab the point on it.
        neighboringPoints.push_back(pointGrid.grid[tile.X][tile.Y]);
    }

    // Return the list of points found in the neighboring tiles.
    return neighboringPoints;
}

/**
 * @brief: Removes all points within a specified section from a given
 * list of points the section falls within.
 * 
 * @param points: The list of points to remove all points within a section from.
 * @param section: The section within which to remove all points.
 */
void PoissonDisc::ExcludeSection(ListOfPoints &points, MapSection section)
{
    // Iterate over all points in the list and delete the ones within the section.
    for (auto pointIterator = points.begin();
         pointIterator != points.end();)
    {
        // Check if the point is within the section area.
        if (InsideSection(*(pointIterator), section))
        {
            // Point is within section area, so remove it from the list, and take
            // new iterator position to continue loop from.
            pointIterator = points.erase(pointIterator);
        }
        else
        {
            // Point no within section area, so continue with the next point.
            pointIterator++;
        }
    }
}

/**
 * @brief: Returns whether a given point falls within a given section.
 * 
 * @param point: The point for which to check whether it falls within the
 * given section.
 * @param section: The section for which to check whether the point falls
 * within it.
 * @return bool: Whether the point falls within the section.
 */
bool PoissonDisc::InsideSection(vector2di point, MapSection section)
{
    // TODO (Gijs): Add support for all MapSection shape types.
    // Currently only eRectangle is supported.
    
    // Derive top left corner offset from section center and dimensions.
    vector2di sectionOffset = 
    {
        section.centerTile.X - section.dimensions.Width / 2,
        section.centerTile.Y - section.dimensions.Height / 2
    };

    // Handle all shape types.
    switch (section.shape)
    {
        case MapSection::eRectangle:
            return InsideRectangle(point, sectionOffset, section.dimensions);

        case MapSection::eEllipse:
            printf("eEllipse map section shape type not yet supported.\n");
            return false;
    }

    // Return false just in case something went wrong.
    return false;
}
