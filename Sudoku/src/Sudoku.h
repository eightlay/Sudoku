#pragma once

#include <string>

class Sudoku
{
public:
	// Constructor
	Sudoku();

	// Deconstructor
	~Sudoku();

	// Read field from path
	void read(char* path);

	// Puzzle to string
	std::string to_string();

	// Print field
	void print();

	// Get cell's value by index
	char get(size_t i, size_t j);

	// Set cell's value by index
	void set(size_t i, size_t j, char val);
	
	// Dimensions
	static const size_t ROWS = 9;
	static const size_t COLS = 9;
	static const size_t DIGITS = 9;
	static const size_t BLOCKS = 9;
	static const size_t CELLS = 81;

private:
	
	// Sudoku unsolved field
	unsigned char** puzzle;
};
