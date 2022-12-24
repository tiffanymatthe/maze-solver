#include <iostream>
#include "Maze.h"
using namespace std;

int main() {
    cout << "Hello world" << endl;
    Maze maze = Maze(30,20);
    // maze.display_maze();
    maze.solveMazeDFS();
    maze.display_maze();
}
