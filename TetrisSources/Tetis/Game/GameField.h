#ifndef __GAMEFIELD_H__
#define __GAMEFIELD_H__

#include "IField.h"

class ObjectManager;

class GameField : public IField
	{
	private:
		ObjectManager* mp_object_manager;

		size_t m_field_width;
		size_t m_field_height;
		float m_cell_size;

		std::vector<bool> m_field;

	private:
		void InitializeField();

	public:
		GameField(const IRect& i_window_rect, size_t i_field_width, size_t i_field_height);
		virtual ~GameField();

		size_t GetWidth() const override { return m_field_width; }
		size_t GetHeight() const override { return m_field_height; }
		
		virtual bool IsCellFree(size_t x, size_t y) const override;
		virtual void OccupyCell(size_t x, size_t y) override;
		virtual void FreeCell(size_t x, size_t y) override;

		virtual void Update(float i_elapsed_time) override;
		virtual void Draw(IRenderer& i_renderer) override;
	};

#endif