#include "SudokuSolver.h"

void SudokuSolver::solve(Sudoku* sudoku, std::string method)
{
	if (method == "recursion")
	{
		recursion_solve(sudoku);
	}
}

bool SudokuSolver::check_possibility(Sudoku* sudoku, size_t i, size_t j, unsigned char n)
{
	for (size_t y = 0; y < Sudoku::COLS; y++)
		if (y != j)
			if (sudoku->get(i, y) == n)
				return false;

	for (size_t x = 0; x < Sudoku::ROWS; x++)
		if (x != i)
			if (sudoku->get(x, j) == n)
				return false;

	const unsigned char b = ITB[i][j];

	for (size_t k = 0; k < Sudoku::DIGITS; k++)
		if (sudoku->get(BDTI[b][k].first, BDTI[b][k].second) == n)
			return false;

	return true;
}

bool SudokuSolver::recursion_solve(Sudoku* sudoku)
{
	for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
			if (sudoku->get(i, j) == 0)
			{
				for (unsigned char n = 1; n < 10; n++)
					if (check_possibility(sudoku, i, j, n))
					{
						sudoku->set(i, j, n);
						if (recursion_solve(sudoku)) return true;
						sudoku->set(i, j, 0);
					}
				return false;
			}
	}
	return true;
}

void SudokuSolver::count_digits(Sudoku* sudoku)
{
	/*for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
		{
			if (puzzle[i][j] != 0)
			{
				rows[i]++;
				cols[j]++;
				blocks[ITB[i][j]]++;
			}
		}
	}*/
}
