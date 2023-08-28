//
// Created by Efe Cinar on 23.07.2023.
//

#ifndef CS_300_HW1_MAZE_H
#define CS_300_HW1_MAZE_H

#include "MazeStack.h"

struct MazeNode{
    // coordinates
    int x, y;
    // doors
    int l, r, d, u;
    // basically the Cells of the Maze
    MazeNode(int row, int column, int l=1, int r=1, int d=1, int u=1): y(row), x(column), l(l), r(r), d(d), u(u){}
};

class Maze{
private:
    int M, N; // row and column numbers of the maze
    MazeStack<MazeNode> GameBoard; // the main stack of the maze(i named it GameBoard) where all the cells are stored
public:
    Maze(int row, int column); // Maze constructor where the GameBoard of MxN size is constructed
    void mazeFileGenerator(int fileNum); // function that prints the content of the maze in file named maze_ID.txt

    /* A function for finding a path from one cell to another in the GameBoard and printing into a file */
    void PathSolver(int startingRow, int startingColumn, int endingRow, int endingColumn, int mazeNum);
};

#endif //CS_300_HW1_MAZE_H
