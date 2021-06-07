#pragma once
#include <iostream>
#include <math.h>

#include "State.h"
#include "Vector.h"

class SlidingPuzzleSolver
{
private:
	// Dimensions
	int size_x, size_y, total_size;

	// Pointers as arrays to hold states
	State* init_state = new State();
	State* final_state = new State();

public:
	// SlidingPuzzleSolver Constructor
	SlidingPuzzleSolver()
	{
		// Basic Initialization
		this->size_x = 3;
		this->size_y = 3;
		this->total_size = this->size_x * this->size_y;

		//AutoInit();	// Solves some random initial configuration
		ManualInit();	// Takes initial state as input from the user
	}

	// Checks solver against random initial state
	void AutoInit()
	{
		/*
		This function solves some initial random configuration
		This function is made for testing purposes only and is not to be
		touched by the user
		*/

		int init_array[] =
		{
			4, 8, 1,
			6, 5, 7,
			0, 2, 3
		};

		int final_array[] =
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 0
		};

		this->init_state->state = Array2Int(init_array);
		this->final_state->state = Array2Int(final_array);

		this->init_state->gcost = 0;
		this->init_state->prev_state = NULL;
		this->final_state->hcost = 0;
		this->final_state->prev_state = NULL;

		// Calculating zero positions
		for (int i = 0; i < this->total_size; i++)
		{
			if (init_array[i] == 0)
			{
				this->init_state->zero_pos[0] = i % this->size_x;
				this->init_state->zero_pos[1] = (i - this->init_state->zero_pos[0]) / this->size_x;
			}

			if (final_array[i] == 0)
			{
				this->final_state->zero_pos[0] = i % this->size_x;
				this->final_state->zero_pos[1] = (i - this->final_state->zero_pos[0]) / this->size_x;
			}
		}
	}

	// Checks the solver against user specified initial state
	void ManualInit()
	{
		/*
		This function takes input and output states from the user
		*/

		/*
		Example puzzles

		// 22 Moves
			4, 8, 1,
			6, 5, 7,
			0, 2, 3

		// 31 Moves A
			8, 6, 7,
			2, 5, 4,
			3, 0, 1

		// 31 Moves B
			6, 4, 7,
			8, 5, 0,
			3, 2, 1

		*/

		// Initializing initial and final state arrays
		int init_array[] =
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 0
		};

		int final_array[] =
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 0
		};

		// Taking initial state input from the user
		std::cout << "\nInstruction: (x, y) :: (0, 0) is top left and (2, 2) is bottom right";
		std::cout << "\nENTER INTIAL STATE:	";
		for (int i = 0; i < this->total_size; i++)
		{
			std::cout << "\nEnter value for (" << i % this->size_x <<
				", " << (i - this->init_state->zero_pos[0]) / this->size_x << "):	";

			std::cin >> init_array[i];
		}

		// Converting state arrays to integer
		this->init_state->state = Array2Int(init_array);
		this->final_state->state = Array2Int(final_array);

		// Initializing costs and previous state pointers from zeros and NULL
		this->init_state->gcost = 0;
		this->init_state->prev_state = NULL;
		this->final_state->hcost = 0;
		this->final_state->prev_state = NULL;

		// Calculating zero positions
		// Keeping track of zero position helps to change the state in O(1)
		for (int i = 0; i < this->total_size; i++)
		{
			if (init_array[i] == 0)
			{
				this->init_state->zero_pos[0] = i % this->size_x;
				this->init_state->zero_pos[1] = (i - this->init_state->zero_pos[0]) / this->size_x;
			}

			if (final_array[i] == 0)
			{
				this->final_state->zero_pos[0] = i % this->size_x;
				this->final_state->zero_pos[1] = (i - this->final_state->zero_pos[0]) / this->size_x;
			}
		}
	}

	bool Feasible()
	{
        int inversions = 0;

		// Convert this->init_state to array
		int state_array[this->total_size - 1];

		int index = 0;
		for(int i=0; i < this->total_size; i++)
		{
			int digit = GetDigit(this->init_state->state, i);
			if(digit > 0)
			{
				state_array[index] = digit;
				index++;
			}
		}

		// Checking the feasibility
        for(int i = 0; i < (this->total_size - 1); i++){
            for(int j = (i + 1); j < (this->total_size - 1); j++){
                if(state_array[j] > state_array[i]){
                    inversions++;
                }
            }
        }

		// Infeasible
        if(inversions%2 == 1){
            return false;
        }
		
		return true;
    }

	// Solves the puzzle using A* algorithm
	bool Solve(int heuristic)
	{
		/*
		This function solves the initial configuration provided by the user
		It tries to reach the final configuration of 1,2,3,4,5,6,7,8,0
		If no solution is found it returns false, else returns true
		*/

		// Adding current state to openList
		Vector<State*> explored;	// Past
		Vector<State*> frontier;	// Current
		State* current_state;

		// Adding init state to frontier
		frontier.append(this->init_state);

		// Keep looking until the frontiers are empty
		while (frontier.size() > 0)
		{
			// Moving the least cost node from frontier to explored list
			int least_cost_index = GetLeastCostStateIndex(&frontier);
			current_state = frontier[least_cost_index];
			explored.append(current_state);
			frontier.deleteAt(least_cost_index);

			// Checking if its the goal
			// If current_state equals the final_state then we have found the solution
			if (current_state->state == this->final_state->state)
			{
				this->final_state->prev_state = current_state->prev_state;
				std::cout << "\nSuccess! Optimal Solution Found";
				return true;
			}

			// Expanding this node
			for (int i = 0; i < 4; i++)
			{
				// Calculating child
				State* successor_state = new State();
				// Copying current state before moving
				successor_state->state = current_state->state;
				successor_state->zero_pos[0] = current_state->zero_pos[0];
				successor_state->zero_pos[1] = current_state->zero_pos[1];

				// If move is valid
				if (MoveState(successor_state, i))
				{
					successor_state->prev_state = current_state;
					successor_state->gcost = current_state->gcost + 1;

					// Calculating heuristic cost
					if (heuristic == 0)
						successor_state->hcost = ManhattanDistanceHeuristic(successor_state);
					else if (heuristic == 1)
						successor_state->hcost = NumMisplacedHeuristic(successor_state);
					else if (heuristic == 2)
						successor_state->hcost = EuclideanDistanceHeuristic(successor_state);
					else
					{
						std::cout << "\nERROR::Specified Heuristic is un-defined";
						return false;
					}

					// Calculating total cost
					successor_state->tcost = successor_state->gcost + successor_state->hcost;

					// Finding the successor state in frontier and explored lists
					int fIndex = Where(&frontier, successor_state);
					int eIndex = Where(&explored, successor_state);

					if (fIndex != -1)
					{
						// successor is in frontier list
						if (successor_state->tcost < frontier[fIndex]->tcost)
						{
							// Replacing frontier node
							frontier[fIndex]->prev_state = current_state;
							frontier[fIndex]->gcost = successor_state->gcost;
							frontier[fIndex]->hcost = successor_state->hcost;
							frontier[fIndex]->tcost = successor_state->tcost;
						}

						// Deleting allocated heap
						delete successor_state;
					}
					else if (eIndex != -1)
					{
						// successor is in explored list
						if (successor_state->tcost < explored[eIndex]->tcost)
						{
							// Removing from explored and putting in frontier
							explored[eIndex]->prev_state = successor_state->prev_state;
							explored[eIndex]->gcost = successor_state->gcost;
							explored[eIndex]->hcost = successor_state->hcost;
							explored[eIndex]->tcost = successor_state->tcost;
							frontier.append(explored[eIndex]);		// Putting into frontiers
							explored.deleteAt(eIndex);	// Removing from explored
						}

						// Deleting allocated heap
						delete successor_state;
					}
					else
					{
						// Adding the successor to frontier list
						frontier.append(successor_state);
					}
				}
			}
		}

		std::cout << "\nFAILED! Could not find a feasible solution";
		return false;
	}

	// Get the ith digit in the integer
	int GetDigit(int number, int digit)
	{
		/*
		This function returns the ith digit in the sequence(number)
		where i is the "digit" in the above parameters
		*/

		return (number / (int)pow(10, this->total_size - 1 - digit)) % 10;
	}

	// Converts array into a single integer
	int Array2Int(int* array)
	{
		/*
		This function converts the array into a single iteger by
		concatenating its elements
		*/

		// Converts arrays of 'total_size' into single integers
		int result = 0;
		for (int i = 0; i < this->total_size; i++)
		{
			result = result * 10 + array[i];
		}

		return result;
	}

	// Displays the problem statement
	void DisplayProblem()
	{
		/*
		This function prints the problem (intial and final state)
		*/

		std::cout << "\n::Initial State::" << std::endl;
		for (int y = 0; y < this->size_y; y++)
		{
			for (int x = 0; x < this->size_x; x++)
			{
				std::cout << "\t[" << x << ", " << y << "]	:" << 
					GetDigit(this->init_state->state, x + y * this->size_x);
			}
			std::cout << std::endl;
		}
		// Double line space
		std::cout << std::endl << std::endl;
		std::cout << "\n::Final State::" << std::endl;
		for (int y = 0; y < this->size_y; y++)
		{
			for (int x = 0; x < this->size_x; x++)
			{
				std::cout << "\t[" << x << ", " << y << "]	:" << 
					GetDigit(this->final_state->state, x + y * this->size_x);
			}
			std::cout << std::endl;
		}
	}

	// Prints the state
	void DisplayState(State* state)
	{
		/*
		This function prints the state
		*/

		std::cout << "\n============================================================\n" << std::endl;
		for (int y = 0; y < this->size_y; y++)
		{
			for (int x = 0; x < this->size_x; x++)
			{
				std::cout << "\t[" << x << ", " << y << "]	:" << GetDigit(state->state, x + y * this->size_x);
			}
			std::cout << std::endl;
		}
		std::cout << "\n============================================================\n" << std::endl;
	}

	// Applies the move to the state instance (in-place)
	bool MoveState(State* state, int move)
	{
		/*
		This function takes a state(pointer) and a move as input and
		then changes the state by applying the move
		This function returns true if the move is valid
		*/

		// Copying into array
		int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
		for (int i = 0; i < total_size; i++)
		{
			array[i] = GetDigit(state->state, i);
		}

		// Apply the move to the state
		switch (move)
		{
		case 0:
			// Up
			if (state->zero_pos[1] > 0)
			{
				// Swapping the zero with the number above
				array[state->zero_pos[0] + state->zero_pos[1] * this->size_x] =
					GetDigit(state->state, state->zero_pos[0] + (state->zero_pos[1] - 1) * this->size_x);
				array[state->zero_pos[0] + (state->zero_pos[1] - 1) * this->size_x] = 0;
				state->state = Array2Int(array);

				// Updating zero pos
				state->zero_pos[1] -= 1;

				return true;
			}
			break;

		case 1:
			// Right
			if (state->zero_pos[0] < (this->size_x - 1))
			{
				// Swapping zero with number on the right
				array[state->zero_pos[0] + state->zero_pos[1] * this->size_x] =
					GetDigit(state->state, (state->zero_pos[0] + 1) + state->zero_pos[1] * this->size_x);
				array[(state->zero_pos[0] + 1) + state->zero_pos[1] * this->size_x] = 0;
				state->state = Array2Int(array);

				// Updating zero pos
				state->zero_pos[0] += 1;

				return true;
			}
			break;

		case 2:
			// Down
			if (state->zero_pos[1] < (this->size_y - 1))
			{
				// Swapping the zero with the number below
				array[state->zero_pos[0] + state->zero_pos[1] * this->size_x] =
					GetDigit(state->state, state->zero_pos[0] + (state->zero_pos[1] + 1) * this->size_x);
				array[state->zero_pos[0] + (state->zero_pos[1] + 1) * this->size_x] = 0;
				state->state = Array2Int(array);

				// Updating zero pos
				state->zero_pos[1] += 1;

				return true;
			}
			break;

		case 3:
			// Left
			if (state->zero_pos[0] > 0)
			{
				// Swapping zero with number on the left
				array[state->zero_pos[0] + state->zero_pos[1] * this->size_x] =
					GetDigit(state->state, (state->zero_pos[0] - 1) + state->zero_pos[1] * this->size_x);
				array[(state->zero_pos[0] - 1) + state->zero_pos[1] * this->size_x] = 0;
				state->state = Array2Int(array);

				// Updating zero pos
				state->zero_pos[0] -= 1;

				return true;
			}
			break;

		default:
			// Raise exception
			std::cout << "\n!!!!!!!!!!!!!!!!! ILLEGAL MOVE !!!!!!!!!!!!!!!!!!!!!!!\n";
			break;
		}

		return false;
	}

	// Number of misplaced tiles heuristic
	int NumMisplacedHeuristic(State* state)
	{
		/*
		Number of misplaced tiles heuristic function
		*/

		int hcost = 0;
		for (int i=0; i<this->total_size; i++)
		{
			if (GetDigit(state->state, i) != GetDigit(this->final_state->state, i))
			{
				hcost += 1;
			}
		}

		return hcost;
	}

	// Manhattan distance heuristic
	int ManhattanDistanceHeuristic(State* state)
	{
		/*
		Manhattan distance heuristic function
		*/

		int hcost = 0;
		for (int i = 0; i < (this->size_x * this->size_y); i++)
		{
			// Comparing with final_state
			for (int j = 0; j < (this->size_x * this->size_y); j++)
			{
				if (GetDigit(state->state, i) == GetDigit(this->final_state->state, j))
				{
					// state x and y
					int xi = i % this->size_x;	int yi = (i - xi) / this->size_x;

					// goal x and y
					int xj = j % this->size_x;	int yj = (j - xj) / this->size_x;

					// Calculating euclidean ditance and adding to cost
					hcost += abs(xi - xj) + abs(yi - yj);
				}
			}
		}

		return hcost;
	}

	// Euclidian distance heuristic
	float EuclideanDistanceHeuristic(State* state)
	{
		/*
		Euclidian distance heuristic function
		*/

		float hcost = 0;
		for (int i = 0; i < (this->size_x * this->size_y); i++)
		{
			// Comparing with final_state
			for (int j = 0; j < (this->size_x * this->size_y); j++)
			{
				if (GetDigit(state->state, i) == GetDigit(this->final_state->state, j))
				{
					// state x and y
					int xi = i % this->size_x;	int yi = (i - xi) / this->size_x;

					// goal x and y
					int xj = j % this->size_x;	int yj = (j - xj) / this->size_x;

					// Calculating euclidean ditance and adding to cost
					hcost += pow(pow(xi - xj, 2) + pow(yi - yj, 2), 0.5);
				}
			}
		}

		return hcost;
	}

	// Returns the index of element if it exists in the List
	// Returns -1 otherwise
	int Where(Vector<State*>* list, State* state)
	{
		/*
		This function finds a given state in the list and returns
		its index in the list
		*/

		// Checks is the list contains the same State.state in O(n)
		for (int i = 0; i < list->size(); i++)
		{
			if ((*list)[i]->state == state->state)
			{
				return i;
			}
		}

		return -1;
	}

	// Returns the element with the least cost inside the List
	int GetLeastCostStateIndex(Vector<State*>* list)
	{
		/*
		This function returns the index of element with the least cost in the list
		*/

		float min_cost = (*list)[0]->tcost;
		int least_cost_index = 0;
		for (int i = 1; i < list->size(); i++)
		{
			if ((*list)[i]->tcost < min_cost)
			{
				min_cost = (*list)[i]->tcost;
				least_cost_index = i;
			}
		}

		return least_cost_index;
	}

	// Displays the final solution (All moves from initial state to final)
	void DisplayMoves()
	{
		/*
		This function displays the final solution
		*/

		// Getting the solution
		Vector<State*> solution;
		State* state = this->final_state;
		while (state->prev_state != NULL)
		{
			solution.append(state);
			state = state->prev_state;
		}

		// total number of moves required
		int moves = solution.size();

		// Displaying solution
		DisplayState(this->init_state);
		while (solution.size() > 0)
		{
			DisplayState(solution[solution.size()-1]);
			solution.deleteAt(solution.size()-1);
		}

		std::cout << "\nTotal moves required	:" << moves << std::endl;
	}
};