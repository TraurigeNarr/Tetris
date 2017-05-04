#include "stdafx.h"

#include "GameManager.h"

#include "TetrisPiece.h"
#include "GameField.h"
#include "IRandomizer.h"
#include "ISolver.h"
#include "IPieceController.h"
#include "PieceType.h"
#include "BasicRandomizer.h"
#include "Problem.h"

#include <time.h>
#include <GameCore/Render/IRenderer.h>

namespace
{
	class RandomizerBase : public IRandomizer
	{
	public:
		virtual std::unique_ptr<TetrisPiece> GetNext(IField& i_field) const override
		{
			return std::unique_ptr<TetrisPiece>(new TetrisPiece(PieceType::O, i_field, SDK::Color(0,100,0, 255).m_color));
		}
	};

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

		virtual void Rotate() override
		{
			
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
	, m_end_game(false)
	, m_restart_game(true)
{}

GameManager::GameManager(const SDK::IRect& i_window_rect)
	: mp_game_field(new GameField(i_window_rect, 10, 20))
	, mp_current(nullptr)
	, mp_solver(nullptr)
	, mp_randomizer(new BasicRandomizer())
	, m_end_game(false)
	, m_restart_game(true)
{
}

GameManager::~GameManager()
{
}

void GameManager::Initialize()
{
	mp_current = mp_randomizer->GetNext(*mp_game_field);
	srand((unsigned int)time(nullptr));
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

void GameManager::TryMatchLines()
{
	std::vector<unsigned int> colors;
	std::vector<bool> filled;

	GameField& filed = static_cast<GameField&>(*mp_game_field);

	const size_t field_width = filed.GetWidth();
	bool line_removed = false;
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
		}
	}
}

void GameManager::TryRestart()
{
	if (m_restart_game)
	{
		mp_current.reset();
		GameField& field = static_cast<GameField&>(*mp_game_field);
		field.InitializeField();
		mp_current = mp_randomizer->GetNext(*mp_game_field);
	}
}

void GameManager::Draw(SDK::IRenderer& i_renderer)
{
	mp_game_field->Draw(i_renderer);
}

void GameManager::Update(float i_elapsed_time)
{
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
				mp_current = mp_randomizer->GetNext(*mp_game_field);
			}
			else
			{
				TryRestart();
			}
		}
	}

	mp_game_field->Update(i_elapsed_time);
}