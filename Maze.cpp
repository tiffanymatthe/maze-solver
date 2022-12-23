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

Maze::GridValue Maze::get_cell(int row, int col) {
    assert_row_col(row, col);
    return grid[get_raw_index(width) * get_raw_index(row) + get_raw_index(col)];
}

void Maze::set_cell(int row, int col, GridValue value) {
    assert_row_col(row, col);
    grid[get_raw_index(width) * get_raw_index(row) + get_raw_index(col)] = value;
}

void Maze::set_wall_value(int row, int col, std::vector<Side> walls, GridValue value) {
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


Maze::GridValue Maze::get_wall_value(int row, int col, Side wall) {
    assert_row_col(row, col);
    switch(wall) {
        case Side::TOP:
            return get_raw(get_raw_index(row) - 1, get_raw_index(col));
            break;
        case Side::BOTTOM:
            return get_raw(get_raw_index(row) + 1, get_raw_index(col));
            break;
        case Side::LEFT:
             return get_raw(get_raw_index(row), get_raw_index(col) - 1);
            break;
        case Side::RIGHT:
             return get_raw(get_raw_index(row), get_raw_index(col) + 1);
            break;
        default:
            throw;
    }
}

void Maze::build_wall(int row, int col, std::vector<Side> walls) {
    set_wall_value(row, col, walls, GridValue::WALL);
}

void Maze::remove_wall(int row, int col, std::vector<Side> walls) {
    set_wall_value(row, col, walls, GridValue::EMPTY);
}

Maze::GridValue Maze::get_raw(int y, int x) {
    assert(y >= 0);
    assert(y < get_raw_index(height));
    assert(x >= 0);
    assert(x < get_raw_index(width));

    return grid[get_raw_index(width) * y + x];
}

void Maze::set_raw(int y, int x, GridValue value) {
    assert(y >= 0);
    assert(y < get_raw_index(height));
    assert(x >= 0);
    assert(x < get_raw_index(width));

    grid[get_raw_index(width) * y + x] = value;
}

void Maze::set_grid_index_if_valid(int index, GridValue value) {
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
            set_raw(y,x,GridValue::WALL);
        }
    }

    for (auto x : xs_to_set) {
        for (int y = 0; y < get_raw_index(height); ++y) {
            set_raw(y,x,GridValue::WALL);
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

    start_location = make_pair(0,0);
    end_location = make_pair(height - 1, width - 1);
}

int Maze::get_raw_index(int s) {
    return s * 2 + 1;
}

void Maze::initialize_random_maze() {
    fill_borders();
    create_maze_randomized_dfs();
}

void Maze::display_maze() {
    cout << "Displaying maze of height : " << height << " and width : " << width << endl;
    for (int row = 0; row < get_raw_index(height); ++row) {
        for (int col = 0; col < get_raw_index(width); ++col) {
            if (get_raw(row, col) == GridValue::WALL) {
                cout << "\u25A0";
            } else if (get_raw(row, col) == GridValue::PATH) {
                cout << "\033[1;31m\u25A0\033[0m";
            } else if (get_raw(row, col) == GridValue::EMPTY) {
                cout << " ";
            } else {
                throw;
            }
        }
        cout << endl;
    }
}

Maze::Maze() : Maze(DEFAULT_SIZE, DEFAULT_SIZE) {}

Maze::Maze(int w, int h) : width(w), height(h) {
    srand(time(NULL));
    cout << "Initializing maze with height: " << height << " and width: " << width << endl;
    grid = make_unique<GridValue[]>(get_raw_index(width) * get_raw_index(height));
    fill(grid.get(), grid.get() + get_raw_index(width) * get_raw_index(height), GridValue::EMPTY);
    initialize_random_maze();
}

Maze::Side Maze::get_wall_between_cells(pair<int,int> cell1, pair<int,int> cell2) {
    pair<int,int> diff = make_pair(cell2.first - cell1.first, cell2.second - cell1.second);

    if (diff.first == 0) {
        // then left or right
        switch (diff.second) {
            case -1:
                return Side::LEFT;
            case 1:
                return Side::RIGHT;
            default:
                cout << "Cells are not adjacent! Diff in cols is " << diff.second << endl; 
                throw;
        }
    } else if (diff.second == 0) {
        // then left or right
        switch (diff.first) {
            case -1:
                return Side::TOP;
            case 1:
                return Side::BOTTOM;
            default:
                cout << "Cells are not adjacent! Diff in rows is " << diff.first << endl; 
                throw;
        }
    } else {
        cout << "Cells are not adjacent. Diff if rows is " << diff.first << " and for cols " << diff.second << endl;
        throw;
    }
}

void Maze::set_wall_between_cells(pair<int,int> cell1, pair<int,int> cell2, GridValue value) {
    pair<int,int> raw1 = make_pair(get_raw_index(cell1.first), get_raw_index(cell1.second));
    pair<int,int> wall_raw = raw1;

    cout << "getting wall between cells" << endl;
    cout << "Cell1:" << cell1.first << ", " << cell1.second << " and Cell2: " << cell2.first << ", " << cell2.second << endl;

    
    Side side = get_wall_between_cells(cell1, cell2);
    
    switch(side) {
        case Side::TOP:
            wall_raw.first -= 1;
            break;
        case Side::BOTTOM:
            wall_raw.first += 1;
            break;
        case Side::LEFT:
            wall_raw.second -= 1;
            break;
        case Side::RIGHT:
            wall_raw.second += 1;
            break;
        default:
            throw;
    }

    cout << "wall y: " << wall_raw.first << " wall x: " << wall_raw.second << endl;

    set_raw(wall_raw.first, wall_raw.second, value);
}

void Maze::solveMazeDFS() {
    // from start index, dfs, keeping history. if you have to backtrack, pop from history
    // final result is what is in history

    stack<pair<int,int>> s;
    map<pair<int,int>, pair<int,int>> history; // key = cell, value = parent cell to get to key
    vector<vector<bool>> visited(height, vector<bool>(width, false));

    map<Side, string> side_to_str_mapping{
        {Side::TOP, "top"},
        {Side::BOTTOM, "bottom"},
        {Side::LEFT, "left"},
        {Side::RIGHT, "right"}
    };

    s.push(start_location);
    
    while (!s.empty()) {
        pair<int,int> curr_cell = s.top();
        s.pop();

        if (visited[curr_cell.first][curr_cell.second]) {
            continue;
        }

        vector<pair<int,int>> neighbors = get_neighbors(curr_cell.first, curr_cell.second);

        int num_added = 0;
        
        for (auto neighbor : neighbors) {
            if (visited[neighbor.first][neighbor.second]) {
                continue;
            }
            Side wall = get_wall_between_cells(curr_cell, neighbor);
            GridValue wall_value = get_wall_value(curr_cell.first, curr_cell.second, wall);

            if (wall_value == GridValue::EMPTY) { // if wall is down
                s.push(neighbor);
                num_added++;
                history[neighbor] = curr_cell;
            }
        }

        visited[curr_cell.first][curr_cell.second] = true;
    }

    // add history cells to grid
    // also add path to no wall locations
    cout << "Len of history " << history.size() << endl;
    pair<int,int> prev{-1,-1};

    pair<int,int> path = end_location;

    cout << "End location " << path.first << ":" << path.second << endl;
    cout << "Parent of end location " << history[path].first << ":" << history[path].second << endl;

    while (prev != start_location) {
        cout << "Current cell " << path.first << ":" << path.second << endl;
        set_cell(path.first, path.second, GridValue::PATH);

        if (prev.first != -1) {
            set_wall_between_cells(path, prev, GridValue::PATH);
        }

        prev = path;
        path = history[path];
    }
}
