#pragma once

#include <vector>
#include <memory>

class Maze {
    int height;
    int width;
    std::pair<int,int> start_location;
    std::pair<int,int> end_location;
    std::unique_ptr<bool[]> grid;
    enum class Side { TOP, LEFT, BOTTOM, RIGHT };

    /*
    * Asserts that row and col inputs are valid.
    */
    void assert_row_col(int row, int col);

    /*
    * Gets the value of the Maze at the specified coordinate.
    * @param row row value
    * @param col col value
    * @return true or false value at specified row and col.
    */
    bool get(int row, int col);

    /*
    * Sets the value of the Maze at the specified coordinate.
    * @param row row value
    * @param col col value
    * @param value: true or false value to set at the specified row + col.
    */
    void set(int row, int col, bool value);

    /*
    * Initializes maze by populating grid by true and false.
    * The grid will have all outer values set to True except for the start and end openings.
    * The grid will have a valid path that leads from the start to the exit.
    * A valid path is defined as a sequence of false cells that are connected by +/- row or col (not both).
    * So no diagonal connections.
    */
    void initialize_random_maze();

    /*
    * Sets all outer borders of mazes to true (walls).
    */
    void fill_borders();

    // returnsd a Pair (row, col) based on inputted Side value.
    std::pair<int,int> get_random_coordinate_based_on_side(Side side);
    
    // Sets random entry and exit points in maze.
    void set_random_entry_and_exit_points();

    // do a randomized bfs to create the maze
    void create_maze_randomized_dfs();

public:
    /**
    * Displays maze to cout stream. 1 = wall, 0 = empty space.
    */
    void display_maze();

    /*
    * Initializes a square Maze object with the DEFAULT_SIZE.
    */
    Maze();

    /*
    * Initializes a maze of specified width and height.

    @param width width of maze grid
    @param heigh height of maze grid
    */
    Maze(int width, int height);
};

extern const int DEFAULT_SIZE;
