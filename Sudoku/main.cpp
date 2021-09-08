#include "main.h"
using namespace std;

void test(int&& i)
{

}

int main()
{
	Sudoku sudoku;

	// Read puzzle to solve
	char puzzle_path[] = "C:/Coding/Documents/Projects/Sudoku/Sudoku/data/sudoku2";
	sudoku.read(puzzle_path);
	sudoku.print();

	// Solve puzzle
	ActionList actions = SudokuSolver::constraint_solve(&sudoku);
	
	// Print manually solved puzzle to check
	char puzzle_path1[] = "C:/Coding/Documents/Projects/Sudoku/Sudoku/data/sudoku2s";
	sudoku.read(puzzle_path1);
	
	// Print actions to solve puzzle if action was right
	for (size_t i = 0; i < actions.size(); i++)
	{
		Action action = actions.at(i);

		action.print(true);

		if (action.digit != sudoku.get(action.index.first, action.index.second))
		{
			cout << "BREAK\n";
			break;
		}
	}
	
	// Print manually solved puzzle to check
	sudoku.print();

	return 0;
}
