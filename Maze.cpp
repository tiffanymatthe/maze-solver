#include "Maze.h"
#include <iostream>
#include <vector>
#include <assert.h>
#include <time.h>

using namespace std;

const int DEFAULT_SIZE = 10;

bool Maze::get(int row, int col) {
    assert_row_col(row, col);
    return grid[width * row + col];
}

void Maze::assert_row_col(int row, int col) {
    assert(row >= 0);
    assert(row < height);
    assert(col >= 0);
    assert(col < width);
}

void Maze::set(int row, int col, bool value) {
    assert_row_col(row, col);
    grid[width * row + col] = value;
}

void Maze::fill_borders() {
    vector<int> rows_to_set{0,height-1};
    vector<int> cols_to_set{0,width-1};

    for (auto row : rows_to_set) {
        for (int col = 0; col < width; ++col) {
            set(row,col,true);
        }
    }

    for (auto col : cols_to_set) {
        for (int row = 0; row < height; ++row) {
            set(row,col,true);
        }
    }
}

pair<int,int> Maze::get_random_coordinate_based_on_side(Side side) {
    switch(side) {
        case Side::TOP:
            return make_pair(0, rand() % width);
        case Side::LEFT:
            return make_pair(rand() % height, 0);
        case Side::BOTTOM:
            return make_pair(height - 1, rand() % width);
        case Side::RIGHT:
            return make_pair(rand() % height, width - 1);
        default:
            throw;
    }
}

void Maze::set_random_entry_and_exit_points() {
    // sselects a random Side enum, assumes there are 4 values in Side
    Side start_side = Side(rand() % 4);
    start_location = get_random_coordinate_based_on_side(start_side);
    set(start_location.first, start_location.second, false);

    Side end_side = Side(rand() % 4);
    end_location = get_random_coordinate_based_on_side(end_side);
    int try_count = 10;
    while (end_location == start_location) {

        if (try_count < 0) {
            throw;
        }

        end_location = get_random_coordinate_based_on_side(end_side);
        try_count -= 1;
    }
    set(end_location.first, end_location.second, false);
}

void Maze::create_maze_randomized_dfs() {
    
}

void Maze::initialize_random_maze() {
    fill_borders();
    set_random_entry_and_exit_points();
    create_maze_randomized_dfs();
}

void Maze::display_maze() {
    cout << "Displaying maze of height : " << height << " and width : " << width << endl;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            cout << get(row, col);
        }
        cout << endl;
    }
}

Maze::Maze() : Maze(DEFAULT_SIZE, DEFAULT_SIZE) {}

Maze::Maze(int width, int height) : width(width), height(height) {
    srand(time(NULL));
    cout << "Initializing maze with height: " << height << " and width: " << width << endl;
    grid = make_unique<bool[]>(width * height);
    fill(grid.get(), grid.get() + width * height, false);
    initialize_random_maze();
}
