#include "main.h"
using namespace std;

int main()
{
	Sudoku sudoku;

	char puzzle_path[] = "C:/Coding/Documents/Projects/Sudoku/Sudoku/data/sudoku0";
	
	sudoku.read_field(puzzle_path);

	sudoku.print_field();

	return 0;
}
