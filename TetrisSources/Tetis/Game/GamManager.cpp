#include "stdafx.h"

#include "GameManager.h"

#include "TetrisPiece.h"
#include "GameField.h"
#include "IRenderer.h"
#include "IRandomizer.h"
#include "ISolver.h"
#include "IPieceController.h"

GameManager::GameManager(std::unique_ptr<IField>&& ip_game_field)
	: mp_game_field(std::move(ip_game_field))
	{}

GameManager::GameManager(const IRect& i_window_rect)
	:	mp_game_field(new GameField(i_window_rect, 10, 15))
	, mp_current(nullptr)
	, mp_next(nullptr)
	, mp_solver(nullptr)
	, mp_randomizer(nullptr)
	{
	}

GameManager::~GameManager()
	{
	}

void GameManager::Draw(IRenderer& i_renderer)
	{
	mp_game_field->Draw(i_renderer);
	}

void GameManager::Update(float i_elapsed_time)
	{
	IPieceController piece_controller;
	if (mp_solver != nullptr)
		mp_solver->Solve(piece_controller, *mp_game_field);
	
	if (mp_current != nullptr)
		{
		mp_current->Update(i_elapsed_time);
		if (mp_current->IsDestroyed())
			{
			mp_current = std::move(mp_next);
			if (mp_randomizer)
				mp_next = mp_randomizer->GetNext();
			}
		}

	mp_game_field->Update(i_elapsed_time);
	}