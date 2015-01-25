#ifndef	__GAMEMANAGER_H__
#define __GAMEMANAGER_H__

struct IField;
class ISolver;
class IRandomizer;
class TetrisPiece;
class IRenderer;

class GameManager
	{
	private:
		std::unique_ptr<IField> mp_game_field;

		std::unique_ptr<IRandomizer> mp_randomizer;
		std::unique_ptr<ISolver> mp_solver;

		std::unique_ptr<TetrisPiece> mp_current;
		std::unique_ptr<TetrisPiece> mp_next;

	public:
		GameManager(std::unique_ptr<IField>&& ip_game_field);
		GameManager(const IRect& i_window_rect);
		~GameManager();

		void SetRandomizer(std::unique_ptr<IRandomizer> ip_randomizer) { mp_randomizer = std::move(ip_randomizer); }
		IRandomizer& GetRandomizer() { return *mp_randomizer; }

		void SetSolver(std::unique_ptr<ISolver> ip_solver) { mp_solver = std::move(ip_solver); }
		ISolver& GetSolver() { return *mp_solver; }

		void Draw(IRenderer& i_renderer);
		void Update(float i_elapsed_time);
	};

#endif