#ifndef __TETRISPIECE_H__
#define __TETRISPIECE_H__

enum class PieceType;
struct IField;
class ISolver;

class TetrisPiece
	{
	private:
		size_t m_shape_array[4][4];
		bool m_move_made;
		int m_next_position[2];
		size_t m_position_x;
		size_t m_position_y;

		size_t m_width;
		size_t m_height;

		PieceType m_type;
		IField& m_field;

		ISolver* mp_solver;

		bool m_destroyed;

		const float m_time_for_one_move;
		float m_current_time;

	private:
		void UpdateField(bool i_free_cells);
		void Initialize();
		void CalculateSize();
		bool CanMove(int i_delta_x, int i_delta_y) const;
		void PrepareMove(float i_elapsed_time);
		void Move();
		void Shift(int i_delta_x, int i_delta_y);

	public:
		TetrisPiece(PieceType i_type, IField& i_field, ISolver* ip_solver);
		virtual ~TetrisPiece();

		void Update(float i_elapsed_time);

		bool IsDestroyed() const { return m_destroyed; }

		bool TurnRight();
		bool TurnLeft();
		bool MoveLeft();
		bool MoveRight();
	};

#endif