#include "main.h"
using namespace std;

int main()
{
	Sudoku sudoku;

	char puzzle_path[] = "C:/Coding/Documents/Projects/Sudoku/Sudoku/data/sudoku0";
	
	sudoku.read_puzzle(puzzle_path);

	sudoku.print_puzzle();
	SudokuSolver::solve(&sudoku, "recursion");
	sudoku.print_puzzle();

	return 0;
}
