#include "Sudoku.h"

#include <fstream>
#include <iostream>

using std::cout;

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

void Sudoku::read_puzzle(char* path)
{
	std::ifstream sudoku;
	sudoku.open(path);

	char digit;
	size_t position = 0;

	while (sudoku >> digit)
	{
		puzzle[position / ROWS][position % ROWS] = std::atoi(&digit);
		position++;
	}

	sudoku.close();
}

void Sudoku::print_puzzle()
{
	cout << "|-----------------------|\n";

	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			if (j % 3 == 0) cout << "| ";

			if (puzzle[i][j] == 0) cout << "  ";
			else cout << +puzzle[i][j] << " ";

		}

		cout << "| \n";
		if ((i + 1) % 3 == 0) cout << "|-----------------------|\n";
	}
}

char Sudoku::get(size_t i, size_t j)
{
	return puzzle[i][j];
}

void Sudoku::set(size_t i, size_t j, char val)
{
	puzzle[i][j] = val;
}
