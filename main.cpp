#include "windows.h"
#include <vector>
#include <iostream>
#include <time.h>
#include <math.h> 
#include <stdio.h>
#include <stdlib.h>
#include "vld.h"

//*****************************************************************************
// NOTE:
//
// Lower value is a better score than a higher value
// Hence the reason most functions check for a lesser value
//
//****************************************************************************

using namespace std;
int main();

#pragma region "Hill Climbing"

//checks whether there are any attacks that can be made
int checkAttack(int *grid, int numQueens) {

	//set score to 0
	int score = 0;

	for (int row = 0; row < numQueens - 1; row++) {
		for (int col = row + 1; col < numQueens; col++) {
			//if value of row is equal to col add to the score
			//checks vertical to see if any collide
			if (grid[row] == grid[col]) {
				score++;
			}

			//if value of row in grid - value of col in grid = row - col add to the score
			//checks diagonal one way to see if any collide
			if (grid[row] - grid[col] == row - col) {
				score++;
			}

			//if calue of row in grid - value of col in grid = col - row add to the score
			//checks diagonal the other way to see if any collide
			if (grid[row] - grid[col] == col - row) {
				score++;
			}

		}
	}
	//if score == 0 then the algorithm is finished
	return score;
}
//draws the final grid with the queens in place
void drawGrid(int *grid, int numQueens) {

	for (int i = 0; i < numQueens; i++) {
		for (int j = 0; j < numQueens; j++) {
			//places the queens
			if (j == grid[i]) {
				cout << " Q ";
			}
			//if the space is not a queen then place an O
			else {
				cout << " - ";
			}
		}
		cout << endl;
	}
}

//makes a new grid from the current state, so makes a new state in the form of a grid
int* generateGrid(int *grid, int numQueens) {

	//create a placement vector
	vector<int> choiceToPlace;


	//sees if there are any attacks and gets the score
	int attack = checkAttack(grid, numQueens);
	//arbitray variable for comparison
	int k;

	//make new grid
	int *gridOut;
	//make it equal to the amount of queens the player chose
	gridOut = new int[numQueens];

	//make gridOut the same as the current grid
	for (int i = 0; i < numQueens; i++) {
		gridOut[i] = grid[i];
	}

	for (int i = 0; i < numQueens; i++) {

		//empty the vector
		choiceToPlace.clear();
		//put the i value into the placement vector
		choiceToPlace.push_back(gridOut[i]);
		//store the value in a temp variable
		//temp = gridOut[i];

		//row
		for (int j = 0; j < numQueens; j++) {

			//set i value of grid to j
			gridOut[i] = j;
			//get value of k through checkAttack function
			k = checkAttack(gridOut, numQueens);
			
			//if k equals attack
			if (k == attack) {

				//put the value of j in the placement vector
				choiceToPlace.push_back(j);
				drawGrid(gridOut, numQueens);

			}
			//if k is less than attack
			if (k < attack) {

				//empty the placement vector
				choiceToPlace.clear();
				//put the j value into the placement vector
				choiceToPlace.push_back(j);
				//set attack to k
				attack = k;

			}
		}
		//i value of gridOut becomes a random placement equal to the size of placement vector
		gridOut[i] = choiceToPlace[rand() % choiceToPlace.size()];
	}
	//return the new grid
	return gridOut;
}



//finds the next place to move to, if there is no place to move to then it will restart with a new grid
bool findNextState(int *grid, int numQueens) {

	//get the original calculated value and set that to mainCalculate
	int isAttack = checkAttack(grid, numQueens);
	//temporary grid
	int *tempGrid;
	//tempGrid is a new board
	
	tempGrid = generateGrid(grid, numQueens);

	//if new checkAttack is less than the original calculated value
	if (checkAttack(tempGrid, numQueens) < isAttack) {
		//fill grid with the value from tempGrid
		for (int p = 0; p < numQueens; p++) {
			//fill grid with tempGrid Values
			grid[p] = tempGrid[p];
		}
		//returns true as it is still searching for the correct answer
		return true;
	}
	//returns false if it has found the solution
	return false;
}

//creates a random grid to work off of
void initialRandomGrid(int *grid, int numQueens) {

	for (int i = 0; i < numQueens; i++) {
		//random placement of the queens
		grid[i] = rand() % numQueens;
	}

}

//Launches the algorithm for solving N-Queens problem using hillclimbing
void solveNQueens(int numQueens) {

	cout << "Thinking about it..." << endl;

	//create the grid int
	int *grid;
	//assign the grid to be a array of sorts
	grid = new int[numQueens];

	//call initialRandomGrid to make a random grid to use
	initialRandomGrid(grid, numQueens);

	//while score != 0, so if score becomes 0 break the loop and draw the finished grid
	while (checkAttack(grid, numQueens) != 0) {
		//if the algorithm cannot find a better state and it is not the goal then restart the grid
		if (findNextState(grid, numQueens) == false) {
			//restart with a random grid to place the queens on
			initialRandomGrid(grid, numQueens);
		}
	}
	cout << endl << "Answer for " << numQueens << " queens:" << endl << endl;
	//draw the final grid with the queens in the correct places
	drawGrid(grid, numQueens);
	system("Pause");
	main();
}

#pragma endregion


#pragma region "Simulated Annealing"


float initialTemp = 1000;
float finalTemp = 10;
float temp = initialTemp;

//makes a new grid from the current state, so makes a new state in the form of a grid
int* generateGridAnneal(int *grid, int numQueens) {


	//placement vector
	vector<int> placement;
	//place to place the queen
	int place = checkAttack(grid, numQueens);
	//tempPlacement
//	int tempPlace;
	//to store the new placement of the queen temp
	int newPlace;
	//create a new output grid
	int* gridOut;
	//fill the grid with values
	gridOut = new int[numQueens];

//	int newCost;

	//make the gridOut the same as the current grid
	for (int i = 0; i < numQueens; i++) {
		gridOut[i] = grid[i];
	}

	while (temp > finalTemp) {
		for (int i = 0; i < numQueens; i++) {
			placement.clear();
			placement.push_back(gridOut[i]);
			//place = gridOut[i];
			for (int j = 0; j < numQueens; j++) {
				gridOut[i] = j;
				newPlace = checkAttack(gridOut, numQueens);
				if (newPlace == place) {
					placement.push_back(j);
				}
				//if the new place is worse then the original place
				else if (newPlace < place) {
					float rando = (rand() / (float)RAND_MAX * 1);
					//temp placement higher means more likely to take the worse option
					if (rando > (exp(checkAttack(gridOut, numQueens) - checkAttack(grid, numQueens) / temp))) {
						placement.push_back(j);
					}
					else { 
						placement.clear();
						placement.push_back(j);
						place = newPlace;
					}
				}
				

			}
			//i value of gridOut becomes a random placement equal to the size of placement vector
			gridOut[i] = placement[rand() % placement.size()];;
		}
		temp = temp - 0.05f;
		cout << temp << endl;

	}
	return gridOut;
}

bool findNextStateAnneal(int *grid, int numQueens) {

	//get the original calculated value and set that to mainCalculate
	int isAttack = checkAttack(grid, numQueens);
	//temporary grid
	int *tempGrid;

	//tempGrid is a new board

	tempGrid = generateGridAnneal(grid, numQueens);

	//if new checkAttack is less than the original calculated value
	if (checkAttack(tempGrid, numQueens) < isAttack) {
		//fill grid with the value from tempGrid
		for (int p = 0; p < numQueens; p++) {
			//fill grid with tempGrid Values
			grid[p] = tempGrid[p];
		}
		return true;
	}
	return false;
}

//simulated anneal algorithm function
void solveNQueensAnneal(int numQueens) {

	cout << "Thinking about annealing..." << endl;

	//creat the grid
	int *grid;
	//fill the grid
	grid = new int[numQueens];
	//create a random placement to start with
	initialRandomGrid(grid, numQueens);
	//while there are still attacks that can be made
	while (checkAttack(grid, numQueens) != 0) {
		if (findNextStateAnneal(grid, numQueens) == false) {
			drawGrid(grid, numQueens);
			system("Pause");
			temp = initialTemp;
			generateGridAnneal(grid, numQueens);
				//cout << "Still Thinking" << endl;
				//initialRandomGrid(grid, numQueens);
		}
	}
	cout << endl << "Answer for " << numQueens << " queens using simulated annealing: " << endl << endl;
	//draws the final grid
	drawGrid(grid, numQueens);
	system("Pause");
	main();

}	

#pragma endregion

#pragma region "Main"
//main function, initialise here
int main() {

	//clear the screen
	system("cls");

	//the number of queens that the player wants to use
	int numQueens;
	//random seed for the placement of the initial grid
	srand(time(NULL));

	//gets the number of queens the player wants to use for this problem
	cout << "Enter number of queens you want to use" << endl;
	cin >> numQueens;
	
	//lets the user have to input something larger than 3 as the n-queens problem doesn't work on 3x3 or less board
	if (numQueens < 4) {
		cout << "Queen number must be greater than 3" << endl;
		system("Pause");
		main();
	}

	//used for which algorithm the user wants to choose
	int choice = 3;
	while (choice >= 2 || choice < 0) {
	cout << "Do you want to use hill climbing(0) or simulated annealing(1)?" << endl;
	cin >> choice;
	
		if (choice == 0) {
			//start the hillclimb algorithm
			solveNQueens(numQueens);
		}
		if (choice == 1) {
			//start the simulated annealing algorithm
			initialTemp = 1000;
			temp = initialTemp;
			solveNQueensAnneal(numQueens);
		}
	}

	system("Pause");
}
#pragma endregion
