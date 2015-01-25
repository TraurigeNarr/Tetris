#ifndef __TETRISPIECE_H__
#define __TETRISPIECE_H__

class TetrisPiece
	{
	private:
		size_t m_shape_array[4][4];

		bool m_destroyed;

	public:
		TetrisPiece();
		virtual ~TetrisPiece();

		void Update(float i_elapsed_time);

		bool IsDestroyed() const { return m_destroyed; }

		bool TurnRight();
		bool TurnLeft();
	};

#endif