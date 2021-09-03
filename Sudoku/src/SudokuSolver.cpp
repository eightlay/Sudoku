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

void SudokuSolver::init_solver_vars(SolverVars* vars)
{
	// Initial values for domains and counters
	for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
		{
			const bool val = vars->sudoku->get(i, j) == Sudoku::DIGITS;

			for (size_t k = 0; k < Sudoku::DIGITS; k++)
			{
				vars->domain[i][j][k] = val;
			}

			vars->cell_counter[i][j] = 0;
			vars->row_counter[i] == 0;
			vars->col_counter[j] == 0;
			vars->block_counter[ITB[i][j]] == 0;
		}
	}

	// Clear domains from restricted digits
	for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
		{
			const size_t val = vars->sudoku->get(i, j);

			if (val != Sudoku::DIGITS)
			{
				const size_t block = ITB[i][j];

				for (size_t k = 0; k < Sudoku::DIGITS; k++)
				{
					vars->domain[k][j][val] = false;
					vars->domain[i][k][val] = false;
					vars->domain[BDTI[block][k].first][BDTI[block][k].second][val] = false;
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
				vars->cell_counter[i][j] += !vars->domain[i][j][k];
			}

			const char val = vars->sudoku->get(i, j) != Sudoku::DIGITS;

			vars->row_counter[i] += val;
			vars->col_counter[j] += val;
			vars->block_counter[ITB[i][j]] += val;
		}
	}
}

void SudokuSolver::reduce(SolverVars* vars, size_t x, size_t y, char val)
{
	for (size_t k = 0; k < Sudoku::DIGITS; k++)
	{
		// Clear domain of new assigned cell
		vars->domain[x][y][k] = false;

		// Decrease cells' counters
		vars->cell_counter[k][y] += vars->domain[k][y][val];
		vars->cell_counter[x][k] += vars->domain[x][k][val];

		// Delete its value from row and column
		vars->domain[k][y][val] = false;
		vars->domain[x][k][val] = false;
	}

	// Decrease counters
	vars->cell_counter[x][y]++;
	vars->row_counter[x]++;
	vars->col_counter[y]++;
	vars->block_counter[ITB[x][y]]++;
}

void SudokuSolver::simplify(SolverVars* vars)
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
				if (vars->cell_counter[k][p] == AlmostFull)
				{
					for (size_t d = 0; d < Sudoku::DIGITS; d++)
					{
						if (vars->domain[k][p][d])
						{
							vars->domain[k][p][d] = false;
							vars->sudoku->set(k, p, d);
							reduce(vars, k, p, d);

							flag = true;
						}
					}
				}
			}

			// Row
			if (vars->row_counter[k] == AlmostFull)
			{
				for (size_t j = 0; j < Sudoku::COLS; j++)
				{
					for (size_t d = 0; d < Sudoku::DIGITS; d++)
					{
						if (vars->domain[k][j][d])
						{
							vars->domain[k][j][d] = false;
							vars->sudoku->set(k, j, d);
							reduce(vars, k, j, d);
							
							flag = true;
						}
					}
				}
			}

			// Column
			if (vars->col_counter[k] == AlmostFull)
			{
				for (size_t i = 0; i < Sudoku::ROWS; i++)
				{
					for (size_t d = 0; d < Sudoku::DIGITS; d++)
					{
						if (vars->domain[i][k][d])
						{
							vars->domain[i][k][d] = false;
							vars->sudoku->set(i, k, d);
							reduce(vars, i, k, d);

							flag = true;
						}
					}
				}
			}

			// Block
			if (vars->block_counter[k] == AlmostFull)
			{
				for (size_t p = 0; p < Sudoku::DIGITS; p++)
				{
					const size_t x = BDTI[k][p].first;
					const size_t y = BDTI[k][p].second;

					for (size_t d = 0; d < Sudoku::DIGITS; d++)
					{
						if (vars->domain[x][y][d])
						{
							vars->domain[x][y][d] = false;
							vars->sudoku->set(x, y, d);
							reduce(vars, x, y, d);

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
	SolverVars vars;
	vars.sudoku = sudoku;

	// Init domains and counters
	init_solver_vars(&vars);

	// Simplify
	simplify(&vars);
}
