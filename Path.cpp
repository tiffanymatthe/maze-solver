#include "Path.h"

using namespace std;

vector<pair<int,int>> Path::get_path_coordinates(){
    return path_coordinates;
}

void Path::add(pair<int,int> coordinate) {
    path_coordinates.push_back(coordinate);
}

void Path::add(int row, int col) {
    path_coordinates.push_back(make_pair(row,col));
}

Path::Path() {}

Path::Path(vector<pair<int,int>> coordinates) : path_coordinates(coordinates) {}
