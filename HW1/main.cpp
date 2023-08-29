// Efe Cinar

#include <iostream>
#include "Maze.h"
#include <cstdlib>
using namespace std;

int main(){
    int M, N, K;
    cout << "Enter the number of mazes: ";
    cin >> K; // number of mazes is represented as K as given in the homework document
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;
    int size = sizeof(Maze(M,N));
    Maze* MazeArray = (Maze*) calloc(K, size); // allocating memory for K mazes

    for (int i=1; i<=K; i++){ // constructing and printing K mazes one by one
        Maze maze(M,N);
        MazeArray[i-1] = maze;
        maze.mazeFileGenerator(i);
    }
    cout << "All mazes are generated." << endl << endl;

    int selectedMazeId;
    cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: ";
    cin >> selectedMazeId;

    int entryRow, entryColumn, exitRow, exitColumn;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> entryColumn >> entryRow;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitColumn >> exitRow;
    Maze selectedMaze = MazeArray[selectedMazeId - 1];
    /* finding a path in the selected maze for the given coordinates which are printed in another file */
    selectedMaze.PathSolver(entryRow, entryColumn, exitRow, exitColumn, selectedMazeId);

    free(MazeArray); // deallocating the dynamically allocated memory

    return 0;
}
