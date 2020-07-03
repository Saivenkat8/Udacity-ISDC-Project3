/**
	helpers.cpp
	Purpose: helper functions which are useful when
	implementing a 2-dimensional histogram filter.
	This file is incomplete! Your job is to make the
	normalize and blur functions work. Feel free to 
	look at helper.py for working implementations 
	which are written in python.
*/
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include "helpers.h"
// #include "debugging_helpers.cpp"
using namespace std;
/**
	TODO - implement this function
    Normalizes a grid of numbers. 
    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.
    @return - a new normalized two dimensional grid where the sum of 
    	   all probabilities is equal to one.
*/
vector< vector <float> > normalize(vector< vector <float> > grid) {
	// todo - your code here
	vector< vector <float> > newgrid;
  	int height = grid.size();
	int width = grid[0].size();
	newgrid = zeros(height, width);
    int totalgrids;
    totalgrids = height*width;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
          newgrid[i][j] = grid[i][j] / totalgrids;
          }
    }
  return newgrid;
}
/**
	TODO - implement this function.
    Blurs (and normalizes) a grid of probabilities by spreading 
    probability from each cell over a 3x3 "window" of cells. This 
    function assumes a cyclic world where probability "spills 
    over" from the right edge to the left and bottom to top. 
    EXAMPLE - After blurring (with blurring=0.12) a localized 
    distribution like this:
    0.00  0.00  0.00 
    0.00  1.00  0.00
    0.00  0.00  0.00 
    would look like this:
	
	0.01  0.02	0.01
	0.02  0.88	0.02
	0.01  0.02  0.01
    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.
	@param blurring - a floating point number between 0.0 and 1.0 
		   which represents how much probability from one cell 
		   "spills over" to it's neighbors. If it's 0.0, then no
		   blurring occurs. 
    @return - a new normalized two dimensional grid where probability 
    	   has been blurred.
*/
vector < vector <float> > blur(vector < vector < float> > grid, float blurring) {
	// your code here
	int height;
    height = grid.size();
    int width;
    width  = grid[0].size();
    float center;
    center = 1.0-blurring;
    float corner;
    corner = blurring / 12.0;
    float adjacent;
    adjacent = blurring / 6.0;
    vector < vector <float> > window;
    vector<float> firstrow;
    firstrow.push_back(corner);
    firstrow.push_back(adjacent);
    firstrow.push_back(corner);
    vector<float> secondrow;
    secondrow.push_back(adjacent);
    secondrow.push_back(center);
    secondrow.push_back(adjacent);
    vector<float> thirdrow;
    thirdrow.push_back(corner);
    thirdrow.push_back(adjacent);
    thirdrow.push_back(corner);
    window.push_back(firstrow);
    window.push_back(secondrow);
    window.push_back(thirdrow);
    vector < vector <float> > newgrid;
    vector<float> singlerow (width,0.0);
    for (int i = 0; i < height; i++) {
        newgrid.push_back(singlerow);
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          float value;
          value = grid[i][j];
          for (int dx = -1; dx < 2; dx++) {
            for (int dy = -1; dy < 2; dy++) {
                float mul;
                mul = window[dx+1][dy+1];
                int i1;
              	int j1;
              	i1 = (((i + dy) % height) + height) % height;
              	j1 = (((j + dx) % width) + width) % width;
                newgrid[i1][j1] += mul * value;
              }
          }
        }
    }
  	return newgrid;
}
/** -----------------------------------------------
#
#
#	You do not need to modify any code below here.
#
#
# ------------------------------------------------- */


/**
    Determines when two grids of floating point numbers 
    are "close enough" that they should be considered 
    equal. Useful for battling "floating point errors".
    @param g1 - a grid of floats
    
    @param g2 - a grid of floats
    @return - A boolean (True or False) indicating whether
    these grids are (True) or are not (False) equal.
*/
bool close_enough(vector < vector <float> > grid1, vector < vector <float> > grid2) {
	int i, j;
	float value1, value2;
	if (grid1.size() != grid2.size()) {
		return false;
	}
	if (grid1[0].size() != grid2[0].size()) {
		return false;
	}
	for (i=0; i<grid1.size(); i++) {
		for (j=0; j<grid1[0].size(); j++) {
			value1 = grid1[i][j];
			value2 = grid2[i][j];
			if (abs(value2-value1) > 0.0001 ) {
				return false;
			}
		}
	}
	return true;
}
bool close_enough(float value1, float value2) { 
	if (abs(value2-value1) > 0.0001 ) {
		return false;
	} 
	return true;
}
/**
    Helper function for reading in map data
    @param s - a string representing one line of map data.
    @return - A row of chars, each of which represents the
    color of a cell in a grid world.
*/
vector <char> read_line(string s) {
	vector <char> row;

	size_t pos = 0;
	string value;
	string space = " ";
	char cell;
	while ((pos = s.find(space)) != std::string::npos) {
		value = s.substr(0, pos);
		s.erase(0, pos + space.length());
		cell = value.at(0);
		row.push_back(cell);
	}
	return row;
}
/**
    Helper function for reading in map data
    @param file_name - The filename where the map is stored.
    @return - A grid of chars representing a map.
*/
vector < vector <char> > read_map(string file_name) {
	ifstream infile(file_name);
	vector < vector <char> > map;
	if (infile.is_open()) {
		char color;
		vector <char> row;		
		string line;
		while (std::getline(infile, line)) {
			row = read_line(line);
			map.push_back(row);
		}
	}
	return map;
}
/**
    Creates a grid of zeros
    For example:
    zeros(2, 3) would return
    0.0  0.0  0.0
    0.0  0.0  0.0
    @param height - the height of the desired grid
    @param width - the width of the desired grid.
    @return a grid of zeros (floats)
*/
vector < vector <float> > zeros(int height, int width) {
	int i, j;
	vector < vector <float> > newgrid;
	vector <float> newrow;

	for (i=0; i<height; i++) {
		newrow.clear();
		for (j=0; j<width; j++) {
			newrow.push_back(0.0);
		}
		newgrid.push_back(newrow);
	}
	return newgrid;
}
// int main() {
// 	vector < vector < char > > map = read_map("maps/m1.txt");
// 	show_grid(map);
// 	return 0;
// }