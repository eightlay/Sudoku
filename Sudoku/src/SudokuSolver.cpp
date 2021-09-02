#include "SudokuSolver.h"

void SudokuSolver::solve(Sudoku* sudoku, std::string method)
{
	if (method == "recursion")
	{
		recursion_solve(sudoku);
	}
	else if (method == "constraint")
	{
		constraint_solve(sudoku);
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

void SudokuSolver::init_domains(
	Sudoku* sudoku, bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS], char cell_counter[Sudoku::ROWS][Sudoku::COLS],
	char row_counter[Sudoku::ROWS], char col_counter[Sudoku::COLS], char block_counter[Sudoku::BLOCKS]
)
{
	// Initial values for domains and counters
	for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
		{
			const bool val = sudoku->get(i, j) == Sudoku::DIGITS;

			for (size_t k = 0; k < Sudoku::DIGITS; k++)
			{
				domain[i][j][k] = val;
			}

			cell_counter[i][j] = 0;
			row_counter[i] == 0;
			col_counter[j] == 0;
			block_counter[ITB[i][j]] == 0;
		}
	}


	// Clear domains from restricted digits
	for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
		{
			const size_t val = sudoku->get(i, j);

			if (val != Sudoku::DIGITS)
			{
				const size_t block = ITB[i][j];

				for (size_t k = 0; k < Sudoku::DIGITS; k++)
				{
					domain[k][j][val] = false;
					domain[i][k][val] = false;
					domain[BDTI[block][k].first][BDTI[block][k].second][val] = false;
				}
			}
		}
	}

	// Count domains' size for cell
	for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
		{
			for (size_t k = 0; k < Sudoku::DIGITS; k++)
			{
				cell_counter[i][j] += !domain[i][j][k];
			}

			const char val = sudoku->get(i, j) != Sudoku::DIGITS;

			row_counter[i] += val;
			col_counter[j] += val;
			block_counter[ITB[i][j]] += val;
		}
	}
}

void SudokuSolver::reduce_domains(
	bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS], char cell_counter[Sudoku::ROWS][Sudoku::COLS],
	char row_counter[Sudoku::ROWS], char col_counter[Sudoku::COLS], char block_counter[Sudoku::BLOCKS],
	size_t x, size_t y, char val
)
{
	for (size_t k = 0; k < Sudoku::DIGITS; k++)
	{
		// Clear domain of new assigned cell
		domain[x][y][k] = false;

		// Decrease cells' counters
		cell_counter[k][y] += domain[k][y][val];
		cell_counter[x][k] += domain[x][k][val];

		// Delete its value from row and column
		domain[k][y][val] = false;
		domain[x][k][val] = false;
	}

	// Decrease counters
	cell_counter[x][y]++;
	row_counter[x]++;
	col_counter[y]++;
	block_counter[ITB[x][y]]++;
}

void SudokuSolver::simplify_domains(
	Sudoku* sudoku, bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS], char cell_counter[Sudoku::ROWS][Sudoku::COLS],
	char row_counter[Sudoku::ROWS], char col_counter[Sudoku::COLS], char block_counter[Sudoku::BLOCKS]
)
{
	bool flag = true;

	while (flag)
	{
		flag = false;

		for (size_t k = 0; k < Sudoku::DIGITS; k++)
		{
			// Cell
			for (size_t p = 0; p < Sudoku::DIGITS; p++)
			{
				if (cell_counter[k][p] == AlmostFull)
				{
					for (size_t d = 0; d < Sudoku::DIGITS; d++)
					{
						if (domain[k][p][d])
						{
							domain[k][p][d] = false;
							sudoku->set(k, p, d);
							reduce_domains(domain, cell_counter, row_counter, col_counter, block_counter, k, p, d);

							flag = true;
						}
					}
				}
			}

			// Row
			if (row_counter[k] == AlmostFull)
			{
				for (size_t j = 0; j < Sudoku::COLS; j++)
				{
					for (size_t d = 0; d < Sudoku::DIGITS; d++)
					{
						if (domain[k][j][d])
						{
							domain[k][j][d] = false;
							sudoku->set(k, j, d);
							reduce_domains(domain, cell_counter, row_counter, col_counter, block_counter, k, j, d);
							
							flag = true;
						}
					}
				}
			}

			// Column
			if (col_counter[k] == AlmostFull)
			{
				for (size_t i = 0; i < Sudoku::ROWS; i++)
				{
					for (size_t d = 0; d < Sudoku::DIGITS; d++)
					{
						if (domain[i][k][d])
						{
							domain[i][k][d] = false;
							sudoku->set(i, k, d);
							reduce_domains(domain, cell_counter, row_counter, col_counter, block_counter, i, k, d);

							flag = true;
						}
					}
				}
			}

			// Block
			if (block_counter[k] == AlmostFull)
			{
				for (size_t p = 0; p < Sudoku::DIGITS; p++)
				{
					const size_t x = BDTI[k][p].first;
					const size_t y = BDTI[k][p].second;

					for (size_t d = 0; d < Sudoku::DIGITS; d++)
					{
						if (domain[x][y][d])
						{
							domain[x][y][d] = false;
							sudoku->set(x, y, d);
							reduce_domains(domain, cell_counter, row_counter, col_counter, block_counter, x, y, d);

							flag = true;
						}
					}
				}
			}
		}
	}
}

void SudokuSolver::constraint_solve(Sudoku* sudoku)
{
	// Domains and counters
	bool domain[Sudoku::ROWS][Sudoku::COLS][Sudoku::DIGITS];
	char cell_counter[Sudoku::ROWS][Sudoku::COLS];
	char row_counter[Sudoku::ROWS] = { 0 };
	char col_counter[Sudoku::COLS] = { 0 };
	char block_counter[Sudoku::BLOCKS] = { 0 };

	// Init domains and counters
	init_domains(sudoku, domain, cell_counter, row_counter, col_counter, block_counter);

	// Simplify
	simplify_domains(sudoku, domain, cell_counter, row_counter, col_counter, block_counter);
}
