#pragma once

#include "PieceType.h"
#include "IField.h"

class Problem
{
public:
	Problem(const IField& i_field, PieceType i_type, size_t x, size_t y)
		: m_field(i_field)
		, m_type(i_type)
		, m_x(x)
		, m_y(y)
	{}

	const IField& GetField() const { return m_field; }
	PieceType GetPieceType() const { return m_type; }
	size_t GetPieceX() const { return m_x; }
	size_t GetPieceY() const { return m_y; }

private:
	const IField& m_field;
	PieceType m_type;
	size_t m_x;
	size_t m_y;
};