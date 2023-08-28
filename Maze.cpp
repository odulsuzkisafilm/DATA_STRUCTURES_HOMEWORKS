 //
// Created by Efe Cinar on 23.07.2023.
//

#include "Maze.h"
#include "MazeStack.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

// a function to print the content of a stack in a reversed order
void recursivePrinter(ofstream & printer, MazeStack<MazeNode> & stack, bool isPathNotTheRawMaze);

// sorting function only to use for the GameBoard
MazeStack<MazeNode> StackSorter(MazeStack<MazeNode> &stack);

Maze::Maze(int row, int column): M(row), N(column){
    GameBoard.push(MazeNode(0, 0));

    MazeStack<MazeNode> MazeGen; // the MazeNode that the Maze Generating algorithm is applied
    MazeGen.push(MazeNode(0, 0));
    while(GameBoard.elementNum() != M*N){ // by this logic, GameBoard will also keep track of the cells that are visited
        MazeNode currentCell = MazeGen.top();
        /* if there are no suitable (non-visited and within bounds) directions to create a Maze cell */
        if(((GameBoard.findMazeCell(currentCell.y, currentCell.x - 1) || currentCell.x == 0)) &&
           (GameBoard.findMazeCell(currentCell.y, currentCell.x + 1) || currentCell.x == N - 1) &&
           (GameBoard.findMazeCell(currentCell.y - 1, currentCell.x) || currentCell.y == 0) &&
           (GameBoard.findMazeCell(currentCell.y + 1, currentCell.x) || currentCell.y == M - 1)){
            MazeGen.pop();
            continue;
        }else {
            int randNum = rand() % 4; // l=0, r=1, d=2, u=3
            while ((randNum == 0 && currentCell.x == 0) || (randNum == 2 && currentCell.y == 0) ||
                   (randNum == 1 && currentCell.x == N - 1) || (randNum == 3 && currentCell.y == M - 1))
            {
                randNum = rand() % 4; // changing the num for a suitable direction
            }
            switch (randNum) {
                case 0: // creating a cell on the left side
                    if (!GameBoard.findMazeCell(currentCell.y, currentCell.x - 1)) {
                        GameBoard.findNReturnCell(currentCell.y, currentCell.x).l = 0;
                        GameBoard.push(MazeNode(currentCell.y, currentCell.x-1, 1, 0, 1, 1));
                        MazeGen.push(MazeNode(currentCell.y, currentCell.x-1, 1, 0, 1, 1));
                    }
                    break;

                case 1: // creating a cell on the right side
                    if (!GameBoard.findMazeCell(currentCell.y, currentCell.x + 1)) {
                        GameBoard.findNReturnCell(currentCell.y, currentCell.x).r = 0;
                        GameBoard.push(MazeNode(currentCell.y, currentCell.x+1, 0, 1, 1, 1));
                        MazeGen.push(MazeNode(currentCell.y, currentCell.x+1, 0, 1, 1, 1));
                    }
                    break;

                case 2: // creating a cell on the down side
                    if (!GameBoard.findMazeCell(currentCell.y - 1, currentCell.x)) {
                        GameBoard.findNReturnCell(currentCell.y, currentCell.x).d = 0;
                        GameBoard.push(MazeNode(currentCell.y-1, currentCell.x, 1, 1, 1, 0));
                        MazeGen.push(MazeNode(currentCell.y-1, currentCell.x, 1, 1, 1, 0));
                    }
                    break;

                case 3: // creating a cell on the up side
                    if (!GameBoard.findMazeCell(currentCell.y + 1, currentCell.x)) {
                        GameBoard.findNReturnCell(currentCell.y, currentCell.x).u = 0;
                        GameBoard.push(MazeNode(currentCell.y+1, currentCell.x, 1, 1, 0, 1));
                        MazeGen.push(MazeNode(currentCell.y+1, currentCell.x, 1, 1, 0, 1));
                    }
                    break;
            }
        }
    }
    GameBoard = StackSorter(GameBoard); // sorting the stack after its creation to make it comparable
}

void Maze::mazeFileGenerator(int fileNum) {
    string fileName = "maze_";
    fileName += to_string(fileNum);
    fileName += ".txt";
    ofstream clerk(fileName);

    MazeStack<MazeNode> copyStack(GameBoard);
    clerk << M << " " << N << "\n";
    recursivePrinter(clerk, copyStack, false); // to print the content of stack in an increasing order
}

void Maze::PathSolver(int startingRow, int startingColumn, int endingRow, int endingColumn, int mazeNum){
    MazeStack<MazeNode> PathGen; // the stack that the pathfinding algorithm will be applied with
    PathGen.push(GameBoard.findNReturnCell(startingRow, startingColumn));
    MazeStack<MazeNode> WrongPath; // the stack that stores the irrelevant cells as the algorithm runs by
    while(PathGen.top().y != endingRow || PathGen.top().x != endingColumn){
        MazeNode currentCell = PathGen.top();
        /* these conditions depict that the direction is not belong to the path
         * i.e. can't passable or tried and returned */
        bool conditionLeft = currentCell.l==1 || PathGen.findMazeCell(currentCell.y, currentCell.x - 1) ||
                             WrongPath.findMazeCell(currentCell.y, currentCell.x - 1);
        bool conditionRight = currentCell.r==1 || PathGen.findMazeCell(currentCell.y, currentCell.x + 1) ||
                              WrongPath.findMazeCell(currentCell.y, currentCell.x + 1);
        bool conditionDown = currentCell.d==1 || PathGen.findMazeCell(currentCell.y - 1, currentCell.x) ||
                             WrongPath.findMazeCell(currentCell.y - 1, currentCell.x);
        bool conditionUp = currentCell.u==1 || PathGen.findMazeCell(currentCell.y + 1, currentCell.x) ||
                           WrongPath.findMazeCell(currentCell.y + 1, currentCell.x);

        if(conditionLeft && conditionRight && conditionDown && conditionUp){ // no available direction
            MazeNode wrongCell = PathGen.topAndPop();
            WrongPath.push(wrongCell);
            continue;
        }else{
            int randNum = rand() % 4; // l=0, r=1, d=2, u=3

            /* these if statements are written to avoid the inefficiency of choosing a random number [0,3] */
            if (conditionLeft && conditionUp && conditionDown) randNum = 1;
            else if(conditionDown && conditionUp && conditionRight) randNum = 0;
            else if(conditionUp && conditionRight && conditionLeft) randNum = 2;
            else if(conditionLeft && conditionRight && conditionDown) randNum = 3;
            else if(conditionDown && conditionRight) randNum = (randNum * 3) % 6;
            else if(conditionRight && conditionLeft) randNum = (rand() % 2) + 2;
            else if(conditionLeft && conditionUp) randNum = (rand() % 2) + 1;
            else if(conditionLeft && conditionDown) randNum = (randNum % 2) * 2 + 1;
            else if(conditionDown && conditionUp) randNum = randNum % 2;
            else if(conditionUp && conditionRight) randNum = (randNum % 2) * 2;
            else{
                /* in this case random number [0,3] can be preferable */
                while ((randNum == 0 && conditionLeft) ||
                       (randNum == 1 && conditionRight) ||
                       (randNum == 2 && conditionDown) ||
                       (randNum == 3 && conditionUp))
                {
                    randNum = rand() % 4;
                }
            }

            switch (randNum) {
                case 0: // add the left cell to the path
                    PathGen.push(GameBoard.findNReturnCell(currentCell.y, currentCell.x - 1));
                    break;

                case 1: // add the right cell to the path
                    PathGen.push(GameBoard.findNReturnCell(currentCell.y, currentCell.x + 1));
                    break;

                case 2: // add the down cell to the path
                    PathGen.push(GameBoard.findNReturnCell(currentCell.y - 1, currentCell.x));
                    break;

                case 3: // add the up cell to the path
                    PathGen.push(GameBoard.findNReturnCell(currentCell.y + 1, currentCell.x));
                    break;
            }
        }
    }

    /* printing into a file step */
    string fileName = "maze_";
    fileName += to_string(mazeNum) += "_path_";
    fileName += to_string(startingColumn) += "_";
    fileName += to_string(startingRow) += "_";
    fileName += to_string(endingColumn) += "_";
    fileName += to_string(endingRow) += ".txt";
    ofstream printer(fileName);
    recursivePrinter(printer, PathGen, true); // to print the PathGen's content from entry to exit
}

void recursivePrinter(ofstream & printer, MazeStack<MazeNode> & stack, bool isPathNotTheRawMaze){
    if (!stack.isEmpty()) {
        MazeNode currentCell = stack.topAndPop();
        recursivePrinter(printer, stack, isPathNotTheRawMaze);
        if (isPathNotTheRawMaze){ // if the function is called after the path is generated for the maze
            printer << currentCell.x << " " << currentCell.y;
        }else{ // if the function is called after the maze is created
            printer << "x=" << currentCell.x << " y=" << currentCell.y << " l=" << currentCell.l << " r="
                    << currentCell.r << " u=" << currentCell.u << " d=" << currentCell.d;
        }
        printer << "\n";
    }
}

MazeStack<MazeNode> StackSorter(MazeStack<MazeNode> &stack){
    MazeStack<MazeNode> tempStack; /* the stack to transfer the content of the parameter stack in order */
    while(!stack.isEmpty()){
        MazeNode tempNode = stack.topAndPop();

        while((!tempStack.isEmpty()) &&
        ((tempStack.top().y > tempNode.y) || ((tempStack.top().y == tempNode.y) && (tempStack.top().x > tempNode.x)))){
            stack.push(tempStack.top());
            tempStack.pop();
        }
        tempStack.push(tempNode);
    }

    return tempStack;
}

