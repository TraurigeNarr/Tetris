#ifndef __ISOLVER_H__
#define __ISOLVER_H__

class Problem;
class IPieceController;

class ISolver
{
public:
	virtual ~ISolver() {}

	virtual void Solve(IPieceController& i_controller, const Problem& i_problem) = 0;
};

#endif