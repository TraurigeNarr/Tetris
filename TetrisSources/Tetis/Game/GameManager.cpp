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
			
		}

	private:
		TetrisPiece& m_controller_piece;
	};
}

GameManager::GameManager(std::unique_ptr<IField>&& ip_game_field)
	: mp_game_field(std::move(ip_game_field))
{}

GameManager::GameManager(const SDK::IRect& i_window_rect)
	: mp_game_field(new GameField(i_window_rect, 10, 20))
	, mp_current(nullptr)
	, mp_solver(nullptr)
	, mp_randomizer(new BasicRandomizer())
	, m_end_game(false)
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

void GameManager::Draw(SDK::IRenderer& i_renderer)
{
	mp_game_field->Draw(i_renderer);
}

void GameManager::Update(float i_elapsed_time)
{
	if (mp_current != nullptr && !mp_current->IsDestroyed())
	{
		if (mp_solver)
		{
			AutomaticController basic_controller(*mp_current);
			IPieceController* controller = mp_pice_controller.get();
			// if no controller was set use default
			if (controller == nullptr)
				controller = &basic_controller;

			Problem problem(*mp_game_field, mp_current->GetType(), mp_current->GetX(), mp_current->GetY());
			mp_solver->Solve(*controller, problem);
		}
		mp_current->Update(i_elapsed_time);
		if (mp_current->IsDestroyed() && CheckField())
			mp_current = mp_randomizer->GetNext(*mp_game_field);
	}

	mp_game_field->Update(i_elapsed_time);
}