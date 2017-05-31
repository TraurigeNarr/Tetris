#include "stdafx.h"

#include "BasicRandomizer.h"

#include "TetrisPiece.h"
#include "PieceType.h"

#include <GameCore/Render/IRenderer.h>



BasicRandomizer::BasicRandomizer()
{
}

BasicRandomizer::~BasicRandomizer()
{

}

void BasicRandomizer::SetParameters(RandomizerParameters i_parameters)
{
	m_parameters = i_parameters;
	m_random.Flush();
	for (int i = 0; i < m_parameters.size(); ++i)
	{
		m_random.AddEvent(i, m_parameters[i].m_probability);
	}
}

std::unique_ptr<TetrisPiece> BasicRandomizer::GetNext(IField& i_field) const
{
	int random = *m_random.GenerateEvent();
	return std::unique_ptr<TetrisPiece>(new TetrisPiece(m_parameters[random].m_piece, i_field, m_parameters[random].m_color));
}