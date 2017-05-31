#ifndef	__GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "IRandomizer.h"

struct IField;
class ISolver;
class IPieceController;
class IRandomizer;
class TetrisPiece;
namespace SDK {
	class IRenderer;
} // SDK

struct LvlSettings
{
	int lvl_number = 0;
	int needed_score = 0;
	RandomizerParameters pieces;
};

class GameManager
	{
	private:
		std::unique_ptr<IField> mp_game_field;

		std::unique_ptr<IRandomizer> mp_randomizer;
		std::unique_ptr<IPieceController> mp_pice_controller;
		std::unique_ptr<ISolver> mp_solver;

		std::unique_ptr<TetrisPiece> mp_current;
		bool m_end_game;
		bool m_restart_game;
		bool m_win;
		bool m_flush_progress = false;
		int m_score;
		
		std::vector<LvlSettings> m_levels;
		int m_current_level = 0;

	private:
		bool CheckField();
		void TryMatchLines();
		void TryRestart();
		void TryMoveNextLevel();

	public:
		GameManager(std::unique_ptr<IField>&& ip_game_field);
		GameManager(const SDK::IRect& i_window_rect);
		virtual ~GameManager();

		void Initialize();
		void Release();

		void SetRandomizer(std::unique_ptr<IRandomizer> ip_randomizer) { mp_randomizer = std::move(ip_randomizer); }
		IRandomizer& GetRandomizer() { return *mp_randomizer; }

		void SetController(std::unique_ptr<IPieceController> ip_controller) { mp_pice_controller = std::move(ip_controller); }
		IPieceController& GetController() { return *mp_pice_controller; }

		void SetSolver(std::unique_ptr<ISolver> ip_solver) { mp_solver = std::move(ip_solver); }
		ISolver& GetSolver() { return *mp_solver; }

		void Draw(SDK::IRenderer& i_renderer);
		void Update(float i_elapsed_time);

		bool IsEndGame() const { return m_end_game; }
	};

#endif