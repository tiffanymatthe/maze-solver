#include "Maze.h"
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <assert.h>
#include <time.h>

/**
 *  0 1
 * 012345
 * xxxxx 0
 * xoxox 1 0
 * xxxxx 2
 * 
 * col 0 = 2 * 0 + 1 = 1
 * col 1 = 2 * 1 + 1 = 3
 * row 0 = 2 * 0 + 1 = 1
 * 
 * cell 0: raw_width * raw_row + raw_col = 5 * 1 + 1 = 6
 * - left = 6 - 1
 * - right = 6 + 1
 * - top = 6 - raw_width = 1
 * - bottom = 6 + raw_width = 11
 * If first cell calculation is accurate, left, right, bottom, top will always be ok.
 * to check if first cell calculation is ok, raw_row must be odd and row_col must be odd.
*/

using namespace std;

const int DEFAULT_SIZE = 10;

void Maze::assert_row_col(int row, int col) {
    assert(row >= 0);
    assert(row < height);
    assert(col >= 0);
    assert(col < width);
}

bool Maze::is_row_col_ok(int row, int col) {
    return row >= 0 && row < height && col >= 0 && col < width;
}

bool Maze::get_cell(int row, int col) {
    assert_row_col(row, col);
    return grid[get_raw_index(width) * get_raw_index(row) + get_raw_index(col)];
}

void Maze::set_cell(int row, int col, bool value) {
    assert_row_col(row, col);
    grid[get_raw_index(width) * get_raw_index(row) + get_raw_index(col)] = value;
}

void Maze::set_wall_value(int row, int col, std::vector<Side> walls, bool value) {
    assert_row_col(row, col);
    int cell_index = get_raw_index(width) * get_raw_index(row) + get_raw_index(col);
    for (auto wall : walls) {
        switch(wall) {
            case Side::TOP:
                set_grid_index_if_valid(cell_index - get_raw_index(width), value);
                break;
            case Side::BOTTOM:
                set_grid_index_if_valid(cell_index + get_raw_index(width), value);
                break;
            case Side::LEFT:
                set_grid_index_if_valid(cell_index - 1, value);
                break;
            case Side::RIGHT:
                set_grid_index_if_valid(cell_index + 1, value);
                break;
            default:
                throw;
        }
    }
}

void Maze::build_wall(int row, int col, std::vector<Side> walls) {
    set_wall_value(row, col, walls, true);
}

void Maze::remove_wall(int row, int col, std::vector<Side> walls) {
    set_wall_value(row, col, walls, false);
}

bool Maze::get_raw(int y, int x) {
    assert(y >= 0);
    assert(y < get_raw_index(height));
    assert(x >= 0);
    assert(x < get_raw_index(width));

    return grid[get_raw_index(width) * y + x];
}

void Maze::set_raw(int y, int x, bool value) {
    assert(y >= 0);
    assert(y < get_raw_index(height));
    assert(x >= 0);
    assert(x < get_raw_index(width));

    grid[get_raw_index(width) * y + x] = value;
}

void Maze::set_grid_index_if_valid(int index, bool value) {
    if (index < 0 || index >= get_raw_index(width) * get_raw_index(height)) {
        return;
    }
    grid[index] = value;
}

void Maze::fill_borders() {
    vector<int> ys_to_set;

    for (int y = 0; y < get_raw_index(height); y += 2) {
        ys_to_set.push_back(y);
    }

    vector<int> xs_to_set;

    for (int x = 0; x < get_raw_index(width); x += 2) {
        xs_to_set.push_back(x);
    }

    for (auto y : ys_to_set) {
        for (int x = 0; x < get_raw_index(width); ++x) {
            set_raw(y,x,true);
        }
    }

    for (auto x : xs_to_set) {
        for (int y = 0; y < get_raw_index(height); ++y) {
            set_raw(y,x,true);
        }
    }
}

pair<int,int> Maze::get_random_coordinate_based_on_side(Side side) {
    switch(side) {
        case Side::TOP:
            return make_pair(0, rand() % width);
            break;
        case Side::LEFT:
            return make_pair(rand() % height, 0);
            break;
        case Side::BOTTOM:
            return make_pair(height - 1, rand() % width);
            break;
        case Side::RIGHT:
            return make_pair(rand() % height, width - 1);
            break;
        default:
            throw;
    }
}

void Maze::set_random_entry_and_exit_points() {
    // sselects a random Side enum, assumes there are 4 values in Side
    // Side start_side = Side(rand() % 4);
    // start_location = get_random_coordinate_based_on_side(start_side);
    // set(start_location.first, start_location.second, false);

    // Side end_side = Side(rand() % 4);
    // end_location = get_random_coordinate_based_on_side(end_side);
    // int try_count = 10;
    // while (end_location == start_location) {

    //     if (try_count < 0) {
    //         throw;
    //     }

    //     end_location = get_random_coordinate_based_on_side(end_side);
    //     try_count -= 1;
    // }
    // set(end_location.first, end_location.second, false);
}

vector<pair<int,int>> Maze::get_neighbors(int row, int col) {
    vector<pair<int,int>> possible_diffs{{-1,0}, {0,1}, {1, 0}, {0,-1}};
    vector<pair<int,int>> neighbors;
    for (auto diff : possible_diffs) {
        if (is_row_col_ok(row + diff.first, col + diff.second)) {
            neighbors.push_back(make_pair(row + diff.first, col + diff.second));
        }
    }

    return neighbors;
}

void Maze::create_maze_randomized_dfs() {
    stack<pair<int,int>> s;
    stack<pair<int,int>> history;
    vector<vector<bool>> visited(height, vector<bool>(width, false));

    // choose initial cell, mark as visited and push to stack
    visited[0][0] = true;
    s.push(make_pair(0,0));

    while (!s.empty()) {
        // while stack is not empty, pop
        pair<int,int> current_cell = s.top();
        s.pop();

        // pick an unvisited neighbor and remove wall between
        vector<pair<int,int>> neighbors = get_neighbors(current_cell.first, current_cell.second);
        vector<pair<int,int>> unvisited_neighbors;

        for (auto neighbor: neighbors) {
            if (!visited[neighbor.first][neighbor.second]) {
                unvisited_neighbors.push_back(neighbor);
            }
        }

        if (unvisited_neighbors.empty()) {
            if (!history.empty()) {
                s.push(history.top());
                history.pop();
            }
            continue;
        }

        pair<int,int> random_unvisited_neighbor = unvisited_neighbors[rand() % unvisited_neighbors.size()];

        pair<int,int> diff = make_pair(
            random_unvisited_neighbor.first - current_cell.first,
            random_unvisited_neighbor.second - current_cell.second
        );

        Side to_remove;

        if (diff.first == 0) {
            // must be a col change
            switch(diff.second) {
                case 1:
                    to_remove = Side::RIGHT;
                    break;
                case -1:
                    to_remove = Side::LEFT;
                    break;
                default:
                    cout << "Col should be -1 or 1, not " << diff.second << endl;
                    throw;
            }
        } else if (diff.second == 0) {
            // must be a row change
            switch(diff.first) {
                case 1:
                    to_remove = Side::BOTTOM;
                    break;
                case -1:
                    to_remove = Side::TOP;
                    break;
                default:
                    cout << "Row should be -1 or 1, not " << diff.first << endl;
                    throw;
            } 
        } else {
            throw;
        }

        map<Side, string> side_to_str_mapping{
            {Side::TOP, "top"},
            {Side::BOTTOM, "bottom"},
            {Side::LEFT, "left"},
            {Side::RIGHT, "right"}
        };

        remove_wall(current_cell.first, current_cell.second, {to_remove});

        // mark that neighbor as visited and push to stack
        visited[random_unvisited_neighbor.first][random_unvisited_neighbor.second] = true;
        s.push(random_unvisited_neighbor);
        history.push(current_cell);
    }

    set_raw(0,1,false);
    set_raw(get_raw_index(height) - 2, get_raw_index(width) - 1, false);
}

int Maze::get_raw_index(int s) {
    return s * 2 + 1;
}

void Maze::initialize_random_maze() {
    fill_borders();
    // set_random_entry_and_exit_points();
    create_maze_randomized_dfs();
}

void Maze::display_maze() {
    cout << "Displaying maze of height : " << height << " and width : " << width << endl;
    for (int row = 0; row < get_raw_index(height); ++row) {
        for (int col = 0; col < get_raw_index(width); ++col) {
            if (get_raw(row, col)) {
                cout << "\u25A0";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

Maze::Maze() : Maze(DEFAULT_SIZE, DEFAULT_SIZE) {}

Maze::Maze(int w, int h) : width(w), height(h) {
    srand(time(NULL));
    cout << "Initializing maze with height: " << height << " and width: " << width << endl;
    grid = make_unique<bool[]>(get_raw_index(width) * get_raw_index(height));
    fill(grid.get(), grid.get() + get_raw_index(width) * get_raw_index(height), false);
    display_maze();
    initialize_random_maze();
}
