#include "stdafx.h"

#include "GameField.h"
#include "TetrisPiece.h"

#include <GameCore/Render/ScopedMatrixTransformation.h>
#include <GameCore/Render/LightsController.h>
#include <GameCore/Core.h>

#include <GameCore/Render/IRenderer.h>

#include <Math/VectorConstructor.h>

using namespace SDK;

namespace
{

	float CalculateCellSize(const IRect& i_window_rect, size_t i_field_width, size_t i_field_height)
	{
		float w = static_cast<float>(i_window_rect.Width());
		float h = static_cast<float>(i_window_rect.Height());

		float x_part = 0.95f*(w / i_field_width);
		float y_part = 0.95f*(h / i_field_height);

		return std::min(x_part, y_part);
	}

}

GameField::GameField(const IRect& i_window_rect, size_t i_field_width, size_t i_field_height)
	: m_field_width(i_field_width)
	, m_field_height(i_field_height)
{
	m_cell_size = CalculateCellSize(i_window_rect, m_field_width, m_field_height);
	InitializeField();
}

void GameField::InitializeField()
{
	const size_t field_size = m_field_width*m_field_height;
	m_field.clear();
	m_field_colors.clear();
	m_field.reserve(field_size);
	m_field_colors.reserve(field_size);
	for (size_t i = 0; i < field_size; ++i)
	{
		m_field.push_back(false);
		m_field_colors.push_back(0);
	}
}

GameField::~GameField()
{}

size_t GameField::GetPosition(size_t x, size_t y) const
{
	size_t row = y*m_field_width;
	size_t position = row + x;
	if (position >= m_field.size())
		throw std::logic_error("Position should be less than field size");
	return position;
}

bool GameField::IsCellFree(size_t x, size_t y) const
{
	return m_field[GetPosition(x, y)] == false;
}

unsigned int GameField::GetColor(size_t x, size_t y) const
{
	return m_field_colors[GetPosition(x, y)];
}

void GameField::OccupyCell(size_t x, size_t y, unsigned int i_color)
{
	size_t pos = GetPosition(x, y);
	m_field[pos] = true;
	m_field_colors[pos] = i_color;
}

void GameField::FreeCell(size_t x, size_t y)
{
	size_t pos = GetPosition(x, y);
	m_field[GetPosition(x, y)] = false;
	m_field_colors[pos] = 0;
}

void GameField::Draw(SDK::IRenderer& i_renderer)
{
	i_renderer.SetProjectionType(SDK::Render::ProjectionType::Orthographic);
	i_renderer.SetMatrixMode(SDK::MatrixMode::Projection);
	
	i_renderer.PushMatrix();
	i_renderer.ModifyCurrentMatrix(Matrix4f::MakeScale(Vector3{ 1.f,-1.f,1.f }));
	i_renderer.GetLightsController()->DisableLighting();
	using namespace SDK::Math;
	auto rect = i_renderer.GetTargetRectangle();
	auto width = static_cast<float>(rect.Width());
	auto height = static_cast<float>(rect.Height());

	Vector2D bottomleft = VectorConstructor<float>::Construct(width / 2, height / 2);
	bottomleft[0] -= (m_cell_size*m_field_width) / 2;
	bottomleft[1] += (m_cell_size*m_field_height) / 2;

	width = m_cell_size*m_field_width;
	height = m_cell_size*m_field_height;

	for (size_t i = 0; i < m_field_height + 1; ++i)
	{
		i_renderer.RenderLine(Vector3{ bottomleft[0], bottomleft[1] - m_cell_size*i, 0.f },
			Vector3{ bottomleft[0] + width, bottomleft[1] - m_cell_size*i, 0.f },
			Color(255, 255, 255, 255));
	}

	for (size_t i = 0; i < m_field_width + 1; ++i)
	{
		i_renderer.RenderLine(VectorConstructor<float>::Construct(bottomleft[0] + m_cell_size*i, bottomleft[1], 0.f),
			VectorConstructor<float>::Construct(bottomleft[0] + m_cell_size*i, bottomleft[1] - height, 0.f),
			Color(255, 255, 255, 255));
	}

	// render cells
	for (size_t i = 0; i < m_field.size(); ++i)
	{
		if (m_field[i])
		{
			int row = i / m_field_width;
			int collumn = i % m_field_width;
			Vector3 center = Vector3{ bottomleft[0] + m_cell_size*(collumn + 0.5f), bottomleft[1] - m_cell_size*(row + 0.5f), 0.f };
			i_renderer.RenderRectangle(center, m_cell_size - 1.f, m_cell_size - 1.f, Color(m_field_colors[i]));
		}
	}
	i_renderer.PopMatrix();
	i_renderer.SetMatrixMode(SDK::MatrixMode::ModelView);
}

void GameField::Update(float i_elapsed_time)
{
}