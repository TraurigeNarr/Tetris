#ifndef __ISOLVER_H__
#define __ISOLVER_H__

class IPieceController;
struct IField;

class ISolver
	{
	public:
		virtual ~ISolver(){}

		virtual void Solve(IPieceController& i_controller, const IField& i_field) = 0;
	};

#endif