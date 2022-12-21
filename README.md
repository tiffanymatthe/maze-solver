# Solves auto-generated maze configurations using various algorithms.

Algorithms:
- DFS
- Prim's algorithm
- Kruskal's algorithm
- Aldous-broder algorithm
- A* search

The idea for this project was generated using ChatGPT.

## Class Design

- Maze: class that holds the state of the maze (just walls, size, visualization methods).
- Path: class that represents the path in the maze (coordinates of path). Will also have timestamps. Can represent multiple paths that are connected at the root.
- Visualizer: takes in Maze and Path objects to visualize the progress.
- Solve (abstract): Represents an algorithm, has a method which will output a path with timestamps.

# Installation with ArchLinux

1. Install gcc with `sudo pacman -Syy gcc`.
2. Download this repository.
3. Inside the repo, run `g++ main.cpp -o main` to compile.
4. Run `./main` to run the program.

`g++ main.cpp Maze.cpp -o main`