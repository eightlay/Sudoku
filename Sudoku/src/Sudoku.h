#pragma once

class Sudoku
{
public:
	// Constructor
	Sudoku();

	// Deconstructor
	~Sudoku();

	// Print field
	void print_field();

	// Read field from path
	void read_field(char* path);

	// Count digits in all rows, cols and blocks
	void count_digits();

private:
	// Dimensions
	const size_t ROWS = 9;
	const size_t COLS = 9;
	const size_t DIGITS = 9;
	const size_t BLOCKS = 9;
	const size_t CELLS = 81;

	// Number of set digits in rows
	unsigned char* rows;

	// Number of set digits in columns
	unsigned char* cols;

	// Number of set digits in blocks
	unsigned char* blocks;

	// Sudoku field
	unsigned char** field;
};

// Block and digit to index (BDTI)
constexpr unsigned char BDTI[9][9] = {
	{0, 1, 2, 9, 10, 11, 18, 19, 20},
	{3, 4, 5, 12, 13, 14, 21, 22, 23},
	{6, 7, 8, 15, 16, 17, 24, 25, 26},
	{27, 28, 29, 36, 37, 38, 45, 46, 47},
	{30, 31, 32, 39, 40, 41, 48, 49, 50},
	{33, 34, 35, 42, 43, 44, 51, 52, 53},
	{54, 55, 56, 63, 64, 65, 72, 73, 74},
	{57, 58, 59, 66, 67, 68, 75, 76, 77},
	{60, 61, 62, 69, 70, 71, 78, 79, 80}
};

// Index to block and digit (ITBD)
constexpr unsigned char ITBD[9][9] = {
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