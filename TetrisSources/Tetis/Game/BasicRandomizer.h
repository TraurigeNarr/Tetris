#ifndef __BASICRANDOMIZER_H__
#define __BASICRANDOMIZER_H__

#include "IRandomizer.h"

class BasicRandomizer : public IRandomizer
	{
	public:
		BasicRandomizer();
		~BasicRandomizer();

		virtual std::unique_ptr<TetrisPiece> GetNext(IField& i_field, ISolver* ip_solver) const override;
	};

#endif