#include "stdafx.h"

#include "SolverImplementation.h"
#include "Game/Problem.h"
#include "Game/IPieceController.h"

SolverImplementation::~SolverImplementation()
{}

void SolverImplementation::Solve(IPieceController& i_controller, const Problem& i_problem)
{
	if (i_problem.GetPieceType() == PieceType::O)
	{
		size_t first_free_x = 0;
		size_t min_y = i_problem.GetField().GetHeight();
		bool found = false;
		for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
		{
			for (size_t j = 0; j < i_problem.GetField().GetHeight(); ++j)
			{
				if (i_problem.GetField().IsCellFree(i, j))
				{
					if (j < min_y)
					{
						first_free_x = i;
						min_y = j;
						found = true;
					}
					break;
				}
			}
		}
		if (found)
		{
			int needed_shifts = (int)first_free_x - (int)i_problem.GetPieceX();
			if (needed_shifts < 0)
			{
				for (int i = 0; i < -needed_shifts; ++i)
					i_controller.Left();
			}
			else
			{
				for (int i = 0; i < needed_shifts; ++i)
					i_controller.Right();
			}
		}
	}
	
}