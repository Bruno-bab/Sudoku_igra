#pragma once

#include "resource.h"
#include <vector>
#include <time.h>
using namespace std;

bool unUsedInBox(vector<vector<int>>& grid, int rowStart, int colStart, int num);

void fillBox(vector<vector<int>>& grid, int row, int col);

bool unUsedInRow(vector<vector<int>>& grid, int i, int num);

bool unUsedInCol(vector<vector<int>>& grid, int j, int num);

bool checkIfSafe(vector<vector<int>>& grid, int i, int j, int num);

void fillDiagonal(vector<vector<int>>& grid);

bool fillRemaining(vector<vector<int>>& grid, int i, int j);

void removeKDigits(vector<vector<int>>& grid, int k);

vector<vector<int>> sudokuGenerator(int k);
