#include <iostream>
#include <chrono> 

#include "SlidingPuzzleSolver.h"

// Special Thanks to Ahmed Mustafa for sharing a source about feasibility
// calculation of an initial state

int main()
{
	// Creating the SlidingPuzzleSolver object
	SlidingPuzzleSolver sps = SlidingPuzzleSolver();
	// Displaying the problem statement
	sps.DisplayProblem();

	// Return if infeasible
	if(!sps.Feasible())
	{
		std::cout << "\nProblem is Infeasible...";
		return 0;
	}

	// Asking the user to specify heuristic
	int heuristic;
	std::cout << "\nPlease select a heuristic..."
		<< "\n	Enter 0 for Manhattan distance heuristic"
		<< "\n 	1 for Number of misplaced tiles heuristic"
		<< "\n 	2 for Euclidian distance heuristic	:";
	std::cin >> heuristic;

	// Recording start time
	auto start = std::chrono::high_resolution_clock::now();
	// Calling the Solve function
	bool solved = sps.Solve(heuristic);
	// Recording end time
	auto stop = std::chrono::high_resolution_clock::now();

	// Display the solution if solved
	if(solved)
		sps.DisplayMoves();

	// Calculating time elaped
	auto duration = std::chrono::duration_cast<
		std::chrono::microseconds>(stop - start);
	std::cout << "\nTime taken by function: "
		<< duration.count()/1000 << " milliseconds" << std::endl;

	// Pausing on completion
	system("pause");

	return 0;
}