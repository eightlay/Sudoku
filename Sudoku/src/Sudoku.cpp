#include "Sudoku.h"

#include <fstream>
#include <iostream>

using std::cout;
using std::endl;

Sudoku::Sudoku()
{
	rows = new unsigned char[ROWS];
	cols = new unsigned char[COLS];
	blocks = new unsigned char[BLOCKS];

	field = new unsigned char* [ROWS];

	for (size_t i = 0; i < ROWS; i++)
	{
		rows[i] = 0;
		cols[i] = 0;
		blocks[i] = 0;

		field[i] = new unsigned char[COLS];

		for (size_t j = 0; j < ROWS; j++)
		{
			field[i][j] = 0;
		}
	}
}

Sudoku::~Sudoku()
{
	delete[] rows;
	delete[] cols;
	delete[] blocks;

	for (size_t i = 0; i < ROWS; i++)
		delete[] field[i];
	delete[] field;
}

void Sudoku::count_digits()
{
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			if (field[i][j] != 0)
			{
				rows[i]++;
				cols[j]++;
				blocks[ITBD[i][j]]++;
			}
		}
	}
}

void Sudoku::print_field()
{
	cout << "|-----------------------|" << endl;

	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			if (j % 3 == 0) cout << "| ";

			if (field[i][j] == 0) cout << "  ";
			else cout << +field[i][j] << " ";
		}

		cout << "| " << endl;
		if ((i + 1) % 3 == 0) cout << "|-----------------------|" << endl;
	}
}

void Sudoku::read_field(char* path)
{
	std::ifstream sudoku;
	sudoku.open(path);

	char digit;
	size_t position = 0;

	while (sudoku >> digit)
	{
		field[position / ROWS][position % ROWS] = std::atoi(&digit);
		position++;
	}

	sudoku.close();

	count_digits();
}