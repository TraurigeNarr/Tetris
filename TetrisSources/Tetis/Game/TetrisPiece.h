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

	unsigned int m_color;

	bool m_wait_drop;

private:
	void UpdateField(bool i_free_cells);
	void Initialize();
	void CalculateSize();
	bool CanMove(int i_delta_x, int i_delta_y) const;
	void PrepareMove(float i_elapsed_time);
	void Move();
	void Shift(int i_delta_x, int i_delta_y);

public:
	TetrisPiece(PieceType i_type, IField& i_field, unsigned int i_color);
	virtual ~TetrisPiece();

	PieceType GetType() const { return m_type; }
	/// Returns left of piece
	size_t GetX() const { return m_position_x; }
	/// Returns bottom of piece
	size_t GetY() const { return m_position_y; }

	void Update(float i_elapsed_time);

	bool IsDestroyed() const { return m_destroyed; }
	
	void SetSolved() { m_move_made = true; }
	bool IsMoveMade() const { return m_move_made; }

	void MoveLeft();
	void MoveRight();
	void Rotate();
	void Drop();
};

#endif