#include "stdafx.h"

#include "GameManager.h"

#include "TetrisPiece.h"
#include "GameField.h"
#include "IRandomizer.h"
#include "ISolver.h"
#include "IPieceController.h"
#include "PieceType.h"
#include "Problem.h"

#include "BasicRandomizer.h"

#include <time.h>
#include <string>
#include <iostream>
#include <GameCore/Render/IRenderer.h>
#include <GameCore/Resources/ResourceManager.h>
#include <GameCore/Core.h>
#include <GameCore/Render/Fonts/FontManager.h>
#include <GameCore/Render/ScopedMatrixTransformation.h>
#include <GameCore/Render/LightsController.h>
#include <GameCore/PropertyReaders.h>

namespace
{
	class AutomaticController : public IPieceController
	{
	public:
		AutomaticController(TetrisPiece& i_controlled_piece)
			: m_controller_piece(i_controlled_piece)
		{}

		virtual void Left() override
		{
			m_controller_piece.MoveLeft();
		}

		virtual void Right() override
		{
			m_controller_piece.MoveRight();
		}

		virtual void Rotate(int i_times) override
		{
			for (int i = 0; i < i_times; ++i)
				m_controller_piece.Rotate();
		}

		virtual void Drop() override
		{
			m_controller_piece.Drop();
		}

	private:
		TetrisPiece& m_controller_piece;
	};
}

GameManager::GameManager(std::unique_ptr<IField>&& ip_game_field)
	: mp_game_field(std::move(ip_game_field))
	, mp_current(nullptr)
	, mp_solver(nullptr)
	, mp_randomizer(new BasicRandomizer())
	, m_end_game(false)
	, m_restart_game(true)
	, m_win(false)
	, m_score(0)
{}

GameManager::GameManager(const SDK::IRect& i_window_rect)
	: mp_game_field(new GameField(i_window_rect, 10, 20))
	, mp_current(nullptr)
	, mp_solver(nullptr)
	, mp_randomizer(new BasicRandomizer())
	, m_end_game(false)
	, m_restart_game(true)
	, m_win(false)
	, m_score(0)
{
}

GameManager::~GameManager()
{
}

TypeParameters CreateTypeParams(const std::string& i_type, int i_probability)
{
	PieceType piece = PieceType::O;
	SDK::Color color(255, 0, 0, 255);
	if (i_type == "O")
	{
		piece = PieceType::O;
		color = SDK::Color(0, 100, 0, 255);
	}
	else if (i_type == "I")
	{
		piece = PieceType::I;
		color = SDK::Color(0, 100, 100, 255);
	}
	else if (i_type == "J")
	{
		piece = PieceType::J;
		color = SDK::Color(0, 10, 230, 255);
	}
	else if (i_type == "L")
	{
		piece = PieceType::L;
		color = SDK::Color(30, 10, 200, 255);
	}
	else if (i_type == "S")
	{
		piece = PieceType::S;
		color = SDK::Color(130, 0, 130, 255);
	}
	else if (i_type == "Z")
	{
		piece = PieceType::Z;
		color = SDK::Color(99, 50, 100, 255);
	}
	else if (i_type == "T")
	{
		piece = PieceType::T;
		color = SDK::Color(132, 10, 12, 255);
	}

	return TypeParameters(piece, i_probability, color.m_color);
}

void GameManager::Initialize()
{
	auto p_load_manager = SDK::Core::GetGlobalObject<SDK::Resources::ResourceManager>();
	p_load_manager->LoadResourceSet("Resources\\ResourceSets\\tetris.res");

	srand((unsigned int)time(nullptr));
	using namespace SDK;
	PropretyReader<(int)ReaderType::SDKFormat> reader;
	PropertyElement root = reader.Parse("Resources\\Configs\\tetris.conf");

	m_levels.clear();
	const auto end = root.end<PropertyElement>();
	for (auto it = root.begin<PropertyElement>(); it != end; ++it)
	{
		if (it.element_name() == "lvl")
		{
			LvlSettings settings;
			settings.lvl_number = it->GetValue<int>("number");
			settings.needed_score = it->GetValue<int>("score");

			const auto end_lvl_props = it->end<PropertyElement>();
			for (auto lvl_props = it->begin<PropertyElement>(); lvl_props != end_lvl_props; ++lvl_props)
			{
				if (lvl_props.element_name() == "piece")
				{
					std::string piece_type = lvl_props->GetValue<std::string>("type");
					int probability = lvl_props->GetValue<int>("probability");
					settings.pieces.emplace_back(CreateTypeParams(piece_type, probability));
				}
			}

			m_levels.emplace_back(settings);
		}
	}

	m_restart_game = root.GetValue<bool>("restart_game");

	m_current_level = root.GetValue<int>("current_level") - 1;
	m_current_level = std::max(0, m_current_level);
	if (m_levels.empty())
	{
		mp_randomizer->SetParameters(RandomizerParameters{ TypeParameters(PieceType::O, 0, SDK::Color(255, 0, 0, 255).m_color) });
	}
	else
	{
		mp_randomizer->SetParameters(m_levels[m_current_level].pieces);
	}

	mp_current = mp_randomizer->GetNext(*mp_game_field);
}

bool GameManager::CheckField()
{
	const size_t up_line = mp_game_field->GetHeight() - 1;
	const size_t field_width = mp_game_field->GetWidth();
	for (size_t i = 0; i < field_width; ++i)
	{
		if (!mp_game_field->IsCellFree(i, up_line))
		{
			m_end_game = true;
			return false;
		}
	}
	return true;
}

void GameManager::TryMoveNextLevel()
{
	if (m_current_level < static_cast<int>(m_levels.size()))
	{
		if (m_levels[m_current_level].needed_score <= m_score)
		{
			const int next_level = m_current_level + 1;
			if (next_level < static_cast<int>(m_levels.size()))
			{
				m_score = 0;
				m_current_level = next_level;
				mp_randomizer->SetParameters(m_levels[m_current_level].pieces);
			}
			else
			{
				m_win = true;
			}
		}
	}
}

void GameManager::TryMatchLines()
{
	std::vector<unsigned int> colors;
	std::vector<bool> filled;

	GameField& filed = static_cast<GameField&>(*mp_game_field);

	const size_t field_width = filed.GetWidth();
	bool line_removed = false;
	int lines_removed_number = 0;
	for (size_t j = 0; j < filed.GetHeight(); ++j)
	{
		if (line_removed)
		{
			j = 0;
			line_removed = false;
		}
		bool all_cells_filled = true;
		for (size_t i = 0; i < filed.GetWidth(); ++i)
		{
			if (filed.IsCellFree(i, j))
			{
				all_cells_filled = false;
			}
		}
		if (all_cells_filled)
		{
			colors = filed.m_field_colors;
			filled = filed.m_field;

			filed.m_field.clear();
			filed.m_field_colors.clear();
			filed.m_field.resize(filed.m_field_width*filed.m_field_height);
			filed.m_field_colors.resize(filed.m_field_width*filed.m_field_height);
			size_t row = j*filed.m_field_width;
			size_t position = row;
			for (size_t i = 0; i < filed.m_field_width*filed.m_field_height - filed.m_field_width; ++i)
			{
				if (i < row)
				{
					filed.m_field[i] = filled[i];
					filed.m_field_colors[i] = colors[i];
				}
				else
				{
					filed.m_field[i] = filled[i + filed.m_field_width];
					filed.m_field_colors[i] = colors[i + filed.m_field_width];
				}
			}

			line_removed = true;
			++lines_removed_number;
		}
	}
	m_score += 100 * lines_removed_number;

	TryMoveNextLevel();
}

void GameManager::TryRestart()
{
	if (m_restart_game)
	{
		mp_current.reset();
		GameField& field = static_cast<GameField&>(*mp_game_field);
		field.InitializeField();

		m_score = 0;
		m_win = false;
		if (m_flush_progress)
		{
			m_current_level = 0;
			mp_randomizer->SetParameters(m_levels[m_current_level].pieces);
		}
		mp_current = mp_randomizer->GetNext(*mp_game_field);
	}
}

void GameManager::Draw(SDK::IRenderer& i_renderer)
{
	using namespace SDK;
	i_renderer.SetProjectionType(SDK::Render::ProjectionType::Orthographic);
	i_renderer.SetMatrixMode(SDK::MatrixMode::Projection);
	
	mp_game_field->Draw(i_renderer);

	auto p_renderer = Core::GetRenderer();
	auto p_lights = p_renderer->GetLightsController();
	p_lights->EnableLighting();

	//Render::ScopedMatrixTransformation scopedMatrix(p_renderer);
	IRect rect = p_renderer->GetTargetRectangle();
	Matrix4f proj = p_renderer->GetMatrix(MatrixMode::Projection);

	p_renderer->SetMatrix(MatrixMode::Projection, Matrix4f::CreateOrtho(0, static_cast<float>(rect.Width()), 0, static_cast<float>(rect.Height())));
	auto arial_font = Core::GetGlobalObject<Resources::ResourceManager>()->GetHandleToResource<Render::Font>("Arial");
	std::wstring message = L"¬аш счЄт: " + std::to_wstring(m_score);
	SDK::Core::GetGlobalObject<Render::FontManager>()->Render({ 30, 850 }, 0.65f, message, arial_font);
	message = L"”ровень: " + std::to_wstring(m_levels[m_current_level].lvl_number);
	static float y = 900;
	SDK::Core::GetGlobalObject<Render::FontManager>()->Render({ 30, y }, 0.65f, message, arial_font);
	p_renderer->SetMatrix(MatrixMode::Projection, proj);
	
	p_renderer->SetMatrixMode(MatrixMode::ModelView);
}

void GameManager::Update(float i_elapsed_time)
{
	if (m_win && mp_current == nullptr)
	{
		TryRestart();
	}
	if (mp_current != nullptr)
	{
		if (mp_solver && !mp_current->IsMoveMade())
		{
			AutomaticController basic_controller(*mp_current);
			IPieceController* controller = mp_pice_controller.get();
			// if no controller was set use default
			if (controller == nullptr)
				controller = &basic_controller;

			Problem problem(*mp_game_field, mp_current->GetType(), mp_current->GetX(), mp_current->GetY());
			mp_solver->Solve(*controller, problem);
			mp_current->SetSolved();
		}
		mp_current->Update(i_elapsed_time);
		if (mp_current->IsDestroyed())
		{
			TryMatchLines();
			if (CheckField())
			{
				if (!m_win)
					mp_current = mp_randomizer->GetNext(*mp_game_field);
				else
					mp_current = nullptr;
			}
			else
			{
				TryRestart();
			}
		}
	}

	mp_game_field->Update(i_elapsed_time);
}