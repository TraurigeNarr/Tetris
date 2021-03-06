#ifndef __SOLVERIMPLEMENTATION_H__
#define __SOLVERIMPLEMENTATION_H__

#include "Game/ISolver.h"

class SolverImplementation : public ISolver
	{
	public:
		virtual ~SolverImplementation();
		virtual void Solve(IPieceController& i_controller, const Problem& i_problem) override;
	};

#endif