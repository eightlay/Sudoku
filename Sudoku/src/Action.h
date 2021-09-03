#pragma once

#include <string>
#include <iostream>
#include <vector>

struct Action
{
	Action(std::string technique_, std::pair<size_t, size_t> index_, char digit_, std::string puzzle_)
	{
		technique = technique_;
		index = index_;
		digit = digit_;
		puzzle = puzzle_;
	}

	~Action() {}

	void print(bool with_puzzle = false) const
	{
		std::cout << num << ") Digit " << digit + 1;
		std::cout << " set to cell (" << index.first + 1 << ", " << index.second + 1 << ")";
		std::cout << " by technique \"" << technique << "\"." << std::endl;

		if (with_puzzle)
		{
			std::cout << std::endl << puzzle << std::endl << std::endl;
		}
	}

	mutable size_t num = 0;
	mutable std::string technique;
	mutable std::pair<size_t, size_t> index;
	mutable char digit;
	mutable std::string puzzle;
};

struct ActionList
{
	std::vector<Action> actions = {};
	
	size_t size() const
	{
		return actions.size();
	}

	void push_back(Action&& action)
	{
		action.num = size();
		actions.push_back(action);
	}

	const Action& at(const size_t pos) const
	{
		return actions.at(pos);
	}
};