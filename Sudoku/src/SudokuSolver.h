#pragma once

#include "Sudoku.h"

#include <string>

struct SolverVars
{
	Sudoku* sudoku;
	
	// Cells' domains
	bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS];

	// Cells' domain sizes
	char cell_counter[Sudoku::ROWS][Sudoku::COLS];

	// Assigned cells in row counter
	char row_counter[Sudoku::ROWS];

	// Assigned cells in column counter
	char col_counter[Sudoku::COLS];

	// Assigned cells in block counter
	char block_counter[Sudoku::BLOCKS];

	// Impossible digits in cells of the row counter
	char row_domain_counter[Sudoku::ROWS][Sudoku::DIGITS];

	// Impossible digits in cells of the column counter
	char col_domain_counter[Sudoku::COLS][Sudoku::DIGITS];

	// Impossible digits in cells of the block counter
	char block_domain_counter[Sudoku::BLOCKS][Sudoku::DIGITS];
};

class SudokuSolver
{
public:
	// Solve puzzle
	static void solve(Sudoku* sudoku, std::string method);

	// Recursion solve
	static bool check_possibility(Sudoku* sudoku, size_t i, size_t j, unsigned char n);

	// Recursion solve
	static bool recursion_solve(Sudoku* sudoku);

	// Constraint solve
	static void constraint_solve(Sudoku* sudoku);

private:
	// Helper function for constraint solve
	static void init_solver_vars(SolverVars* vars);

	static void reduce(SolverVars* vars, size_t x, size_t y, char val);

	static void simplify(SolverVars* vars);

	// Almost full row/col/block size
	static const size_t AlmostFull = 8;

	// Amount of neighbours of each block
	static const size_t Neightbours = 4;

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
};