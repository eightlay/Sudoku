#include "main.h"
using namespace std;

int main()
{
	Sudoku sudoku;

	char puzzle_path[] = "C:/Coding/Documents/Projects/Sudoku/Sudoku/data/sudoku0";
	
	sudoku.read_puzzle(puzzle_path);

	sudoku.print_puzzle();
	SudokuSolver::solve(&sudoku, "constraint");
	sudoku.print_puzzle();

	char puzzle_path1[] = "C:/Coding/Documents/Projects/Sudoku/Sudoku/data/sudoku1";

	sudoku.read_puzzle(puzzle_path1);

	sudoku.print_puzzle();

	return 0;
}
