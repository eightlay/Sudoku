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

#pragma region RECURSION SOLVE

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

#pragma endregion RECURSION SOLVE

#pragma region CONSTRAINT SOLVE

void SudokuSolver::init_solver_vars(SolverVars* vars)
{
	// Ensure counters' values are zeros
	for (size_t k = 0; k < Sudoku::DIGITS; k++)
	{
		// Row and column
		vars->row_counter[k] = 0;
		vars->col_counter[k] = 0;

		for (size_t p = 0; p < Sudoku::DIGITS; p++)
		{
			// Cell and block
			vars->cell_counter[k][p] = 0;
			vars->block_counter[ITB[k][p]] = 0;

			// Row, column and block domains counters
			vars->row_domain_counter[p][k] = 0;
			vars->col_domain_counter[p][k] = 0;
			vars->block_domain_counter[p][k] = 0;
		}
	}

	// Initial values for domains
	for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
		{
			// True means that cells's value is assigned
			const bool val = vars->sudoku->get(i, j) == Sudoku::DIGITS;

			for (size_t k = 0; k < Sudoku::DIGITS; k++)
			{
				vars->domain[i][j][k] = val;
			}
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

	// Update counters to initial puzzle values
	for (size_t i = 0; i < Sudoku::ROWS; i++)
	{
		for (size_t j = 0; j < Sudoku::COLS; j++)
		{
			for (size_t v = 0; v < Sudoku::DIGITS; v++)
			{
				if (!vars->domain[i][j][v])
				{
					// Update cells', rows', columns' and blocks' domains counters
					vars->cell_counter[i][j]++;
					vars->row_domain_counter[i][v]++;
					vars->col_domain_counter[j][v]++;
					vars->block_domain_counter[ITB[i][j]][v]++;
				}
			}

			// Update rows', columns' and blocks' counters
			const char val = vars->sudoku->get(i, j) != Sudoku::DIGITS;

			vars->row_counter[i] += val;
			vars->col_counter[j] += val;
			vars->block_counter[ITB[i][j]] += val;
		}
	}
}

void SudokuSolver::reduce(SolverVars* vars, size_t x, size_t y, char val)
{
	// Index to block
	const char block = ITB[x][y];

	for (size_t k = 0; k < Sudoku::DIGITS; k++)
	{
		// Clear domain of new assigned cell
		vars->domain[x][y][k] = false;

		// Increase intersected cells' counters
		vars->cell_counter[k][y] += vars->domain[k][y][val];
		vars->cell_counter[x][k] += vars->domain[x][k][val];

		// Delete its value from intersected row and column
		vars->domain[k][y][val] = false;
		vars->domain[x][k][val] = false;

		// Increase intersected rows' and columns' domains counters
		vars->row_domain_counter[k][val]++;
		vars->col_domain_counter[k][val]++;

		// Increase intersected blocks' domains counters

		for (size_t p = 0; p < Neightbours; p++)
		{
			vars->block_domain_counter[BNB[block][p]][val]++;
		}
	}

	// Increase own counters
	vars->cell_counter[x][y]++;
	vars->row_counter[x]++;
	vars->col_counter[y]++;
	vars->block_counter[block]++;

	// Set own domains counters to max
	vars->row_domain_counter[x][val] = Sudoku::DIGITS;
	vars->col_domain_counter[y][val] = Sudoku::DIGITS;
	vars->block_domain_counter[block][val] = Sudoku::DIGITS;
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

#pragma endregion CONSTRAINT SOLVE

