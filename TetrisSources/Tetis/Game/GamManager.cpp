#include "stdafx.h"

#include "GameManager.h"

#include "TetrisPiece.h"
#include "GameField.h"
#include "IRenderer.h"
#include "IRandomizer.h"
#include "ISolver.h"
#include "IPieceController.h"
#include "PieceType.h"

namespace
	{
	class RandomizerBase : public IRandomizer
		{
		public:
			virtual std::unique_ptr<TetrisPiece> GetNext(IField& i_field, ISolver* ip_solver) const override
				{
				return std::unique_ptr<TetrisPiece>(new TetrisPiece(PieceType::O, i_field, ip_solver));
				}
		};
	}

GameManager::GameManager(std::unique_ptr<IField>&& ip_game_field)
	: mp_game_field(std::move(ip_game_field))
	{}

GameManager::GameManager(const IRect& i_window_rect)
	:	mp_game_field(new GameField(i_window_rect, 10, 15))
	, mp_current(nullptr)
	, mp_solver(nullptr)
	, mp_randomizer(new RandomizerBase())
	{
	}

GameManager::~GameManager()
	{
	}

void GameManager::Initialize()
	{
	mp_current = mp_randomizer->GetNext(*mp_game_field, mp_solver.get());
	}

bool GameManager::CheckField()
	{
	const size_t up_line = mp_game_field->GetHeight() - 1;
	const size_t f_width = mp_game_field->GetWidth();
	for (size_t i = 0; i < f_width; ++i)
		{
		if (!mp_game_field->IsCellFree(i, up_line))
			return false;
		}
	return true;
	}

void GameManager::Draw(IRenderer& i_renderer)
	{
	mp_game_field->Draw(i_renderer);
	}

void GameManager::Update(float i_elapsed_time)
	{	
	if (mp_current != nullptr)
		{
		mp_current->Update(i_elapsed_time);
		if (mp_current->IsDestroyed() && CheckField())
			mp_current = mp_randomizer->GetNext(*mp_game_field, mp_solver.get());
		}

	mp_game_field->Update(i_elapsed_time);
	}