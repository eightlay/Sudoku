#pragma once

#include "Sudoku.h"
#include "Action.h"

#include <string>

struct SolverVars
{
	Sudoku* sudoku;
	
	// Cells' domains
	bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS];

	// Cells' domain sizes
	unsigned char cell_counter[Sudoku::ROWS][Sudoku::COLS];

	// Assigned cells in row counter
	unsigned char row_counter[Sudoku::ROWS];

	// Assigned cells in column counter
	unsigned char col_counter[Sudoku::COLS];

	// Assigned cells in block counter
	unsigned char block_counter[Sudoku::BLOCKS];

	// Impossible digits in cells of the row counter
	unsigned char row_domain_counter[Sudoku::ROWS][Sudoku::DIGITS];

	// Impossible digits in cells of the column counter
	unsigned char col_domain_counter[Sudoku::COLS][Sudoku::DIGITS];

	// Impossible digits in cells of the block counter
	unsigned char block_domain_counter[Sudoku::BLOCKS][Sudoku::DIGITS];

	// Actions to solve puzzle
	ActionList actions;
};

class SudokuSolver
{
public:
	// Solve puzzle
	static void solve(Sudoku* sudoku, std::string method);

	// Recursion solve
	static bool recursion_solve(Sudoku* sudoku);

	// Constraint solve
	static ActionList constraint_solve(Sudoku* sudoku);

private:
#pragma region Recursion solve helper functions

	// Check if it is possible to assign n in cell (i, j)
	static bool check_possibility(Sudoku* sudoku, size_t i, size_t j, unsigned char n);

#pragma endregion

#pragma region Helper function for constraint solve

	// Initialize data for constraint solver
	static void init_solver_vars(SolverVars* vars);

	// Reduce domains after assigning val to cell (x, y)
	static void reduce(SolverVars* vars, size_t x, size_t y, char val);

	// Naked single technique
	static bool naked_single(SolverVars* vars, size_t anchor); 

	// Full house technique
	static bool full_house(SolverVars* vars, size_t anchor); 

	// Hidden single technique
	static bool hidden_single(SolverVars* vars, size_t anchor); 

	// Simplify puzzle with all available technuques
	// while there is simplifications
	static void simplify(SolverVars* vars);

#pragma endregion

	// Almost full row/col/block size
	static const size_t AlmostFull = 8;

	// Number of neighbours of each block
	static const size_t Neightbours = 4;
	static const size_t DirectionNeightbours = 2;

	// Number of intersected cells of block's neighbours
	static const size_t BNIntersectedCells = 3;

	// Number of directions
	static const size_t Directions = 2;

	// Vertical and horizontal codes
	static const size_t horizontal = 0;
	static const size_t vertical = 1;

	// Number of rows and columns in block
	static const size_t BlockLines = 3;

	// Block and digit to index
	static constexpr std::pair<unsigned char, unsigned char> BDTI[Sudoku::ROWS][Sudoku::COLS] = {
		{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}},
		{{0, 3}, {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}},
		{{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}},
		{{3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 1}, {5, 2}},
		{{3, 3}, {3, 4}, {3, 5}, {4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 4}, {5, 5}},
		{{3, 6}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {5, 8}},
		{{6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 1}, {8, 2}},
		{{6, 3}, {6, 4}, {6, 5}, {7, 3}, {7, 4}, {7, 5}, {8, 3}, {8, 4}, {8, 5}},
		{{6, 6}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 6}, {8, 7}, {8, 8}}
	};

	// Index to block (ITB)
	static constexpr unsigned char ITB[Sudoku::ROWS][Sudoku::COLS] = {
		{0, 0, 0, 1, 1, 1, 2, 2, 2},
		{0, 0, 0, 1, 1, 1, 2, 2, 2},
		{0, 0, 0, 1, 1, 1, 2, 2, 2},
		{3, 3, 3, 4, 4, 4, 5, 5, 5},
		{3, 3, 3, 4, 4, 4, 5, 5, 5},
		{3, 3, 3, 4, 4, 4, 5, 5, 5},
		{6, 6, 6, 7, 7, 7, 8, 8, 8},
		{6, 6, 6, 7, 7, 7, 8, 8, 8},
		{6, 6, 6, 7, 7, 7, 8, 8, 8}
	};

	// Blocks' neighbour blocks
	static constexpr unsigned char BNB[Sudoku::BLOCKS][Neightbours] = {
		{1, 2, 3, 6},
		{0, 2, 4, 7},
		{0, 1, 5, 8},
		{4, 5, 0, 6},
		{3, 5, 1, 7},
		{3, 4, 2, 8},
		{7, 8, 0, 3},
		{6, 8, 1, 4},
		{6, 7, 2, 5}
	};

	// Intersected cells of block's neighbours
	static constexpr unsigned char BNI[Directions][BlockLines][BNIntersectedCells] = {
		{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}},
		{{0, 3, 6}, {1, 4, 7}, {2, 5, 8}}
	};
};