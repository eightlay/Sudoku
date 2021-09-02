#pragma once

#include "Sudoku.h"

#include <string>

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
	static void init_domains(
		Sudoku* sudoku, bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS], char cell_counter[Sudoku::ROWS][Sudoku::COLS],
		char row_counter[Sudoku::ROWS], char col_counter[Sudoku::COLS], char block_counter[Sudoku::BLOCKS]
	);

	static void reduce_domains(
		bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS], char cell_counter[Sudoku::ROWS][Sudoku::COLS],
		char row_counter[Sudoku::ROWS], char col_counter[Sudoku::COLS], char block_counter[Sudoku::BLOCKS],
		size_t x, size_t y, char val
	);

	static void simplify_domains(
		Sudoku* sudoku, bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS], char cell_counter[Sudoku::ROWS][Sudoku::COLS],
		char row_counter[Sudoku::ROWS], char col_counter[Sudoku::COLS], char block_counter[Sudoku::BLOCKS]
	);

	// Almost full row/col/block size
	static const size_t AlmostFull = 8;

	// Block and digit to index (BDTI)
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

	// Row and column to block (ITB)
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
};