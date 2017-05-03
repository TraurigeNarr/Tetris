#ifndef __FIELD_H__
#define __FIELD_H__

namespace SDK {
	class IRenderer;
}

/**
Interface IField - represents rectangular field of cells.
Each cell can be either free or occupied.
IMPORTANT: zero y means bottom of the field
*/
struct IField
	{
	virtual ~IField() {}

	/// Returns width of the field
	virtual size_t GetWidth() const = 0;

	/// Returns height of the field
	virtual size_t GetHeight() const = 0;

	/// Tests whether cell is free
	virtual bool IsCellFree(size_t x, size_t y) const = 0;

	/// Marks cell as occupied
	virtual void OccupyCell(size_t x, size_t y) = 0;

	/// Marks cell as free
	virtual void FreeCell(size_t x, size_t y) = 0;

	virtual void Update(float i_elapsed_time) = 0;
	virtual void Draw(SDK::IRenderer& i_renderer) = 0;
	};

#endif