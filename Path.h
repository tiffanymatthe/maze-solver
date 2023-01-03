#pragma once

#include <vector>

class Path {
    std::vector<std::pair<int,int>> path_coordinates;

public:
    /**
     * Returns path coordinates as a vector of (row,col).
    */
    std::vector<std::pair<int,int>> get_path_coordinates();

    /**
     * Add to coordinates, must be (row,col)
    */

   void add(std::pair<int,int> coordinate);

    /**
        * Add to coordinates.
    */

    void add(int x, int y);

    /**
     * Initialized a Path object with an empty vector of path coordinates.
    */
    Path();

    /**
     * Initializes a Path object with the passed in coordinates.
    */
    Path(std::vector<std::pair<int,int>> coordinates);
};