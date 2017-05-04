#ifndef __IPIECECONTROLLER_H__
#define __IPIECECONTROLLER_H__

/**
Interface IPieceController - controls current piece
*/
class IPieceController
{
public:
	virtual ~IPieceController() {}

	/// Move the piece one cell left
	virtual void Left() = 0;

	/// Move the piece one cell right
	virtual void Right() = 0;

	/// Rotate the piece 90 degree clockwise (see contest documentation for details)
	virtual void Rotate() = 0;

	/// Drop the piece
	virtual void Drop() = 0;
};

#endif