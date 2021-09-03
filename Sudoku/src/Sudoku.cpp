#include "Sudoku.h"

#include <fstream>
#include <iostream>

Sudoku::Sudoku()
{
	puzzle = new unsigned char* [ROWS];

	for (size_t i = 0; i < ROWS; i++)
	{
		puzzle[i] = new unsigned char[COLS];

		for (size_t j = 0; j < ROWS; j++)
		{
			puzzle[i][j] = 0;
		}
	}
}

Sudoku::~Sudoku()
{
	for (size_t i = 0; i < ROWS; i++)
	{
		delete[] puzzle[i];
	}

	delete[] puzzle;
}

void Sudoku::read(char* path)
{
	std::ifstream sudoku;
	sudoku.open(path);

	char digit;
	size_t position = 0;

	while (sudoku >> digit)
	{
		char val = std::atoi(&digit);
		val = (val == 0) ? 9 : val - 1;

		puzzle[position / ROWS][position % ROWS] = val;
		position++;
	}

	sudoku.close();
}

std::string Sudoku::to_string()
{
	std::string result = "    1 2 3   4 5 6   7 8 9\n";
	result += "  |-----------------------|\n";

	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			if (j % 3 == 0)
			{
				if (j / 3 == 0)
				{
					result += std::to_string(i + 1) + " ";
				}

				result += "| ";
			}

			if (puzzle[i][j] == DIGITS)
			{
				result += "  ";
			}
			else
			{
				result += std::to_string(puzzle[i][j] + 1);
				result += " ";
			}
		}

		result += "| \n";
		if ((i + 1) % 3 == 0)
		{
			result += "  |-----------------------|\n";
		}
	}

	return result;
}

void Sudoku::print()
{
	std::cout << std::endl << to_string() << std::endl;
}

char Sudoku::get(size_t i, size_t j)
{
	return puzzle[i][j];
}

void Sudoku::set(size_t i, size_t j, char val)
{
	puzzle[i][j] = val;
}
