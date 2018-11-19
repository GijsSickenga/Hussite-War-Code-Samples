/**
 * @brief: Contains the PoissonDisc class header information.
 * @file PoissonDisc.h
 * @author Gijs Sickenga
 * @date 17-04-2018
 */

#pragma once

#define _USE_MATH_DEFINES
#include <vector>
#include <queue>
#include <limits>

#include "Grid.h"
#include "CustomRandom.h"

using irr::core::dimension2du;
using irr::core::vector2di;

// Typedef some collections to make the code more readable.
typedef std::vector<vector2di> ListOfPoints;
typedef std::vector<ListOfPoints> GridOfPoints;
typedef std::queue<vector2di> QueueOfPoints;

/**
 * @brief: Contains variables and functions for generating a uniformly
 * distributed list of points, for use in the level generation algorithm.
 * 
 * For reference on how Poisson disc sampling works, read this article:
 * http://devmag.org.za/2009/05/03/poisson-disk-sampling/
 */
class PoissonDisc
{
public:
    /**
     * @brief: Defines a section of the poisson disc generation space.
     * Described by a shape, center position, and X & Y dimensions.
     */
    struct MapSection
    {
        enum Shape
        {
            eRectangle,
            eEllipse
        };

        Shape shape;
        vector2di centerTile;
        dimension2du dimensions;
    };

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
    static ListOfPoints GeneratePoints(int minimumDistance,
                                       dimension2du mapDimensions,
                                       const std::vector<MapSection> &excludedSections = {},
                                       const ListOfPoints &existingPoints = {},
                                       const int maxNumberOfPoints = MAX_INT);

  private:
    /**
     * @brief: Shorthand class containing a grid of points, and the dimensions
     * of that grid.
     */
    struct PointGrid2D
    {
        GridOfPoints grid;
        dimension2du dimensions;
    };

    static const int MAX_INT = 2147483647;

    /**
     * @brief: Returns the grid tile the given point is on.
     * 
     * @param point: The point to check the grid tile for.
     * @param cellSize: The size of a grid cell.
     * @return vector2di: The x and y indices of the grid tile the point is on.
     */
    static vector2di PointToGridTile(vector2di point, float cellSize);

    /**
     * @brief: Generates a random point around the given point, between the
     * given minimum distance and twice the minimum distance of the source point.
     * 
     * @param point: The point around which to generate the new point.
     * @param minimumDistance: The minimum distance from the source point to generate
     * the new point.
     * @return vector2di: The newly generated point.
     */
    static vector2di GenerateRandomPointAroundPoint(vector2di point, int minimumDistance);

    /**
     * @brief: Returns whether the given point is inside the given rectangle.
     * 
     * @param point: The point for which to check whether it is inside the rectangle.
     * @param offset: The top left corner offset of the rectangle.
     * @param dimensions: The dimensions of the rectangle.
     * @return bool: Whether the point is inside the rectangle.
     */
    static bool InsideRectangle(vector2di point, vector2di offset, dimension2du dimensions);

    /**
     * @brief: Returns whether a point is already present on the point grid that is
     * less than the minimum distance between points away from the given point.
     * 
     * @param point: The point to check for close neighbors for.
     * @param minimumDistance: The minimum distance between points.
     * @param pointGrid: The grid to check for neighboring points on.
     * @param cellSize: The cell size of the point grid.
     * @return bool: Whether there are any points within minimum distance or less
     * from the given point.
     */
    static bool InNeighborhood(vector2di point,
                               int minimumDistance,
                               PointGrid2D pointGrid,
                               float cellSize);

    /**
     * @brief: Returns a list of points around the given tile, within the given
     * tile range.
     * 
     * @param centerTile: The tile to return a list of closely neighboring points for.
     * @param pointGrid: The point grid to check for points on.
     * @return ListOfPoints: The list of closely neighboring points.
     */
    static ListOfPoints GetPointsAroundTile(vector2di centerTile, PointGrid2D pointGrid);

    /**
     * @brief: Removes all points within a specified section from a given
     * list of points the section falls within.
     * 
     * @param points: The list of points to remove all points within a section from.
     * @param section: The section within which to remove all points.
     */
    static void ExcludeSection(ListOfPoints &points, MapSection section);

    /**
     * @brief: Returns whether a given point falls within a given section.
     * 
     * @param point: The point for which to check whether it falls within the
     * given section.
     * @param section: The section for which to check whether the point falls
     * within it.
     * @return bool: Whether the point falls within the section.
     */
    static bool InsideSection(vector2di point, MapSection section);
};
