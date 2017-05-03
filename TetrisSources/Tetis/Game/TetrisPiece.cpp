#include "stdafx.h"

#include "TetrisPiece.h"
#include "IField.h"
#include "ISolver.h"
#include "IPieceController.h"
#include "PieceType.h"

TetrisPiece::TetrisPiece(PieceType i_type, IField& i_field, ISolver* ip_solver)
	: m_destroyed(false)
	, m_type(i_type)
	, m_field(i_field)
	, mp_solver(ip_solver)
	, m_time_for_one_move(.1f)
	, m_current_time(.0f)
	{
	Initialize();
	m_next_position[0] = 0;
	m_next_position[1] = 0;
	}

TetrisPiece::~TetrisPiece()
	{
	mp_solver = nullptr;
	}

void TetrisPiece::Shift(int i_delta_x, int i_delta_y)
	{
	m_position_x += i_delta_x;
	m_position_y += i_delta_y;
	}

void TetrisPiece::CalculateSize()
	{
	m_width = 0;
	m_height = 0;
	// should be reverse cycle -> 4 .. 0
	for (size_t i = 0; i < 4; ++i)
		for (size_t j = 0; j < 4; ++j)
			if (m_shape_array[i][j] != 0)
				{
				m_width = i;
				m_height = std::max(j, m_height);
				}
	++m_width;
	++m_height;
	}

void TetrisPiece::Initialize()
	{
	for (size_t i = 0; i < 4; ++i)
		for (size_t j = 0; j < 4; ++j)
			m_shape_array[i][j] = 0;
	
	switch (m_type)
		{
		case PieceType::O:
			m_shape_array[0][0] = 1;
			m_shape_array[1][0] = 1;
			m_shape_array[0][1] = 1;
			m_shape_array[1][1] = 1;
			break;
		case PieceType::L:
			m_shape_array[0][0] = 1;
			m_shape_array[1][0] = 1;
			m_shape_array[0][1] = 1;
			m_shape_array[0][2] = 1;
			break;
		case PieceType::I:
			m_shape_array[0][0] = 1;
			m_shape_array[0][1] = 1;
			m_shape_array[0][2] = 1;
			m_shape_array[0][3] = 1;
			break;
		case PieceType::J:
			m_shape_array[0][0] = 1;
			m_shape_array[1][0] = 1;
			m_shape_array[1][1] = 1;
			m_shape_array[1][2] = 1;
			break;
		case PieceType::S:
			m_shape_array[0][0] = 1;
			m_shape_array[0][1] = 1;
			m_shape_array[1][1] = 1;
			m_shape_array[1][2] = 1;
			break;
		case PieceType::Z:
			m_shape_array[0][1] = 1;
			m_shape_array[0][2] = 1;
			m_shape_array[1][0] = 1;
			m_shape_array[1][1] = 1;
			break;
		case PieceType::T:
			m_shape_array[0][0] = 1;
			m_shape_array[1][0] = 1;
			m_shape_array[2][0] = 1;
			m_shape_array[1][1] = 1;
			break;
		}

	m_position_x = 0;
	m_position_y = 0;
	CalculateSize();
	Shift((m_field.GetWidth()-m_width)/2, m_field.GetHeight()-m_height);
	}

bool TetrisPiece::CanMove(int i_delta_x, int i_delta_y) const
	{
	for (size_t i = 0; i < 4; ++i)
		{
		for (size_t j = 0; j < 4; ++j)
			{
			const int position_x = i + m_position_x + i_delta_x;
			const int position_y = j + m_position_y + i_delta_y;
			if (position_x < 0 || position_x + m_width > m_field.GetWidth()
				|| (position_y-j) + m_height > m_field.GetHeight() || position_y < 0)
				return false;
			if (m_shape_array[i][j] != 0)
				{
				if (!m_field.IsCellFree(position_x, position_y))
					return false;
				else
					break;
				}
			}
		}

	return 0 != m_position_y;
	}

void TetrisPiece::PrepareMove(float i_elapsed_time)
	{
	m_current_time += i_elapsed_time;
	if (m_current_time < m_time_for_one_move)
		return;
	m_current_time = .0f;
	if (!CanMove(0, -1))
		{
		m_destroyed = true;
		return;
		}
	m_next_position[0] = 0;
	m_next_position[1] = -1;
	}

void TetrisPiece::Move()
	{
	if (m_next_position[0] == 0 && m_next_position[1] == 0)
		return;
	m_position_x += m_next_position[0];
	m_position_y += m_next_position[1];
	m_next_position[0] = 0;
	m_next_position[1] = 0;
	}

void TetrisPiece::UpdateField(bool i_free_cells)
	{
	for (size_t i = 0; i < 4; ++i)
		{
		for (size_t j = 0; j < 4; ++j)
			{
			if (j + m_position_y > m_field.GetHeight() - 1)
				continue;
			if (m_shape_array[i][j] != 0)
				{
				if (i_free_cells)
					m_field.FreeCell(i + m_position_x, j + m_position_y);
				else
					m_field.OccupyCell(i + m_position_x, j + m_position_y);
				}
			}
		}
	}

void TetrisPiece::Update(float i_elapsed_time)
	{
	// prepare move and caculate can we move or not
	PrepareMove(i_elapsed_time);
	// free cells that this piece occupy
	UpdateField(true);
	// make real movement
	Move();
	// occupy cells (may be same or changed)
	UpdateField(false);
	// solve problem (input from user or AI)
	// will be in next frame
	IPieceController piece_controller;
	if (mp_solver != nullptr)
		mp_solver->Solve(piece_controller, m_field);
	}

bool TetrisPiece::TurnRight()
	{
	return false;
	}

bool TetrisPiece::TurnLeft()
	{
	return false;
	}