#include "stdafx.h"

#include "BasicRandomizer.h"

#include "TetrisPiece.h"
#include "PieceType.h"

#include <GameCore/Render/IRenderer.h>

BasicRandomizer::BasicRandomizer()
{
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::O, false, SDK::Color(0, 100, 0, 255).m_color));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::L, false, SDK::Color(30, 100, 0, 255).m_color));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::I, false, SDK::Color(0, 100, 12, 255).m_color));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::J, false, SDK::Color(0, 10, 0, 255).m_color));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::S, false, SDK::Color(130, 0, 130, 255).m_color));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::Z, false, SDK::Color(99, 50, 100, 255).m_color));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::T, false, SDK::Color(132, 10, 12, 255).m_color));
}

BasicRandomizer::~BasicRandomizer()
{

}

std::unique_ptr<TetrisPiece> BasicRandomizer::GetNext(IField& i_field) const
{
	int random = rand() % m_parameters.m_type_for_generation.size();

	return std::unique_ptr<TetrisPiece>(new TetrisPiece(m_parameters.m_type_for_generation[random].m_piece, i_field, m_parameters.m_type_for_generation[random].m_color));
}