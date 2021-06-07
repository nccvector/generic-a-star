#pragma once

struct State
{
	int state;			// Integer representation of the state
	State* prev_state;	// Previous state pointer
	float gcost;		// Move cost
	float hcost;		// Heuristic cost
	float tcost;		// Total cost
	int zero_pos[2];	// Zero postion in 2D (x, y)
};