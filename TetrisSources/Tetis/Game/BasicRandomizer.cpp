#include "stdafx.h"

#include "BasicRandomizer.h"

#include "TetrisPiece.h"
#include "PieceType.h"

BasicRandomizer::BasicRandomizer()
	{
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::O, false));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::L, false));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::I, false));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::J, false));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::S, false));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::Z, false));
	m_parameters.m_type_for_generation.push_back(RandomizerParameters::TypeParameters(PieceType::T, false));
	}

BasicRandomizer::~BasicRandomizer()
	{

	}

std::unique_ptr<TetrisPiece> BasicRandomizer::GetNext(IField& i_field, ISolver* ip_solver) const
	{
	int random = rand() % m_parameters.m_type_for_generation.size();

	return std::unique_ptr<TetrisPiece>(new TetrisPiece(m_parameters.m_type_for_generation[random].m_piece, i_field, ip_solver));
	}