#include "stdafx.h"

#include "SolverImplementation.h"
#include "Game/Problem.h"
#include "Game/IPieceController.h"


namespace
{
	class Figure
	{
	public:
		virtual int GetXPos() = 0;
		virtual int GetRotation() = 0;
	};
	/////////////////////////////////////////////////////////////////////////

	class OFigure : public Figure
	{
	private:
		const IField& m_IField;

		bool _IsOverlapping(int xpos, int y_pos)
		{
			if (y_pos == 0)
				return false;
			return m_IField.IsCellFree(xpos, y_pos - 1) || m_IField.IsCellFree(xpos + 1, y_pos - 1);
		}
	public:
		OFigure(const IField& i_IField)
			: m_IField(i_IField)
		{		}

		virtual int GetXPos() override
		{
			int x_pos = 0;

			for (int y_pos = 0; y_pos < (int)m_IField.GetHeight(); ++y_pos)
			{
				for (x_pos = 0; x_pos < (int)m_IField.GetWidth() - 1; ++x_pos)
				{
					if (m_IField.IsCellFree(x_pos, y_pos) && m_IField.IsCellFree(x_pos + 1, y_pos) && !_IsOverlapping(x_pos, y_pos))
						return x_pos;
				}
			}

			return x_pos;
		}

		virtual int GetRotation() override
		{
			return 0;
		}
	};

	//////////////////////////////////////////////////////////////////////////

	class IFigure : public Figure
	{
	private:
		const IField& m_IField;
	public:
		IFigure(const IField& i_IField)
			: m_IField(i_IField)
		{		}

		virtual int GetXPos() override
		{
			int x_pos = 0;

			for (int y_pos = 0; y_pos < (int)m_IField.GetHeight(); ++y_pos)
			{
				for (x_pos = 0; x_pos < (int)m_IField.GetWidth(); ++x_pos)
				{
					if (m_IField.IsCellFree(x_pos, y_pos))
						return x_pos;
				}
			}

			return x_pos;
		}

		virtual int GetRotation() override
		{
			return 0;
		}
	};

	//////////////////////////////////////////////////////////////////////////

	class JFigure : public Figure
	{
	private:
		const IField& m_IField;

		bool _IsOverlapping(int xpos, int y_pos)
		{
			if (y_pos == 0)
				return false;
			return m_IField.IsCellFree(xpos, y_pos - 1) || m_IField.IsCellFree(xpos - 1, y_pos - 1);
		}
	public:
		JFigure(const IField& i_IField)
			: m_IField(i_IField)
		{		}

		virtual int GetXPos() override
		{
			int x_pos = 0;

			for (int y_pos = 0; y_pos < (int)m_IField.GetHeight(); ++y_pos)
			{
				for (x_pos = 1; x_pos < (int)m_IField.GetWidth(); ++x_pos)
				{
					if (m_IField.IsCellFree(x_pos - 1, y_pos) && m_IField.IsCellFree(x_pos, y_pos) && !_IsOverlapping(x_pos, y_pos))
						return x_pos;
				}
			}

			return x_pos;
		}

		virtual int GetRotation() override
		{
			return 0;
		}
	};

	//////////////////////////////////////////////////////////////////////////

	std::unique_ptr<Figure> _GetFigureSolver(PieceType i_type, const IField& i_IField)
	{
		std::unique_ptr<Figure> solver;
		switch (i_type)
		{
			case PieceType::O:
				solver = std::make_unique<OFigure>(i_IField);
				break;
			case PieceType::I:
				solver = std::make_unique<IFigure>(i_IField);
				break;
			case PieceType::J:
				solver = std::make_unique<JFigure>(i_IField);
				break;
		}

		return solver;
	}
}


namespace FirstLevel
{
	void Solve(IPieceController& i_controller, const Problem& i_problem)
	{
		if (i_problem.GetField().IsCellFree(0, 0))
		{
			i_controller.Left();
			i_controller.Left();
			i_controller.Left();
			i_controller.Left();
		}
		else if (i_problem.GetField().IsCellFree(2, 0))
		{
			i_controller.Left();
			i_controller.Left();
		}
		else if (i_problem.GetField().IsCellFree(4, 0))
		{
			// Here we should only drop
		}
		else if (i_problem.GetField().IsCellFree(6, 0))
		{
			i_controller.Right();
			i_controller.Right();
		}
		else if (i_problem.GetField().IsCellFree(8, 0))
		{
			i_controller.Right();
			i_controller.Right();
			i_controller.Right();
			i_controller.Right();
		}
		i_controller.Drop();
	}
}

namespace SecondLevel
{
	void Solve(IPieceController& i_controller, const Problem& i_problem)
	{
		int position = 0;
		for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
		{
			if (i_problem.GetPieceType() == PieceType::O)
			{
				if ((i + 1) < i_problem.GetField().GetWidth() 
					&& i_problem.GetField().IsCellFree(i, 0) 
					&& i_problem.GetField().IsCellFree(i + 1, 0))
				{
					position = i;
					break;
				}
			}
			else if (i_problem.GetField().IsCellFree(i, 0))
			{
				position = i;
				break;
			}
		}
	
		int shift = i_problem.GetPieceX() - position;

		if (shift > 0)
		{
			for (int i = 0; i < shift; ++i)
				i_controller.Left();
		}
		else
		{
			shift *= -1;
			for (int i = 0; i < shift; ++i)
				i_controller.Right();
		}

		i_controller.Drop();
	}
}

namespace field_utils
{
	// check if collumn is empty above than @firsy_y
	bool free_collumn(int x, int first_y, const IField& i_field)
	{
		for (size_t j = first_y; j < i_field.GetHeight(); ++j)
		{
			if (!i_field.IsCellFree(x, j))
			{
				return false;
			}
		}
		return true;
	}
	int first_free_row(int x, const IField& i_field)
	{
		int possible_j = 0;
		for (size_t j = 0; j < i_field.GetHeight(); ++j)
		{
			if (i_field.IsCellFree(x, j))
			{
				bool success = true;
				for (size_t k = j + 1; k < i_field.GetHeight(); ++k)
				{
					if (!i_field.IsCellFree(x, k))
					{
						success = false;
						break;
					}
				}
				if (success)
					return j;
			}
		}
		return 0;
	}
}

namespace ThirdLevel
{
	void check_min_row(int& min_j, int& pos, int x, int count, const IField& i_field, bool check_empty_below)
	{
		bool found_all = false;

		int row = field_utils::first_free_row(x, i_field);
		for (int k = 1; k < count; ++k)
		{
			int possible_row = field_utils::first_free_row(x + k, i_field);
			if (possible_row != row)
			{
				found_all = false;
			}
			if (possible_row != row && check_empty_below)
			{
				row = i_field.GetHeight();
			}
			else
			{
				row = std::max(row, possible_row);
			}
		}

		if (!found_all && !check_empty_below)
		{
			int best_row = field_utils::first_free_row(x, i_field);
			for (int k = 1; k < count; ++k)
			{
				int possible_row = field_utils::first_free_row(x + k, i_field);
				best_row = std::max(best_row, possible_row);
			}
			row = std::min(best_row, row);
		}

		if (min_j > row)
		{
			min_j = row;
			pos = x;
		}
	}

	int Solve_O(IPieceController& i_controller, const Problem& i_problem)
	{
		int position = -1;
		int min_j = i_problem.GetField().GetHeight();

		for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
		{
			if ((i + 1) < i_problem.GetField().GetWidth()
				&& i_problem.GetField().IsCellFree(i, 0)
				&& i_problem.GetField().IsCellFree(i + 1, 0))
			{
				if (field_utils::free_collumn(i, 0, i_problem.GetField())
					&& field_utils::free_collumn(i + 1, 0, i_problem.GetField()))
				{
					position = i;
					break;
				}
				else
				{
					check_min_row(min_j, position, i, 2, i_problem.GetField(), true);
				}
			}
			else if (i + 1 < i_problem.GetField().GetWidth())
			{
				check_min_row(min_j, position, i, 2, i_problem.GetField(), true);
			}
		}

		int min_level_x = i_problem.GetField().GetWidth();
		int min_level_y = i_problem.GetField().GetHeight();
		for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
		{
			if (min_level_y > field_utils::first_free_row(i, i_problem.GetField()))
			{
				min_level_x = i;
				min_level_y = field_utils::first_free_row(i, i_problem.GetField());
			}
		}

		int pos_j = position != -1 ? field_utils::first_free_row(position, i_problem.GetField()) : 0;
		if (position == -1 || (min_level_y < pos_j && pos_j > 10))
		{
			min_j = i_problem.GetField().GetHeight();
			for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
			{
				if ((i + 1) < i_problem.GetField().GetWidth())
				{
					check_min_row(min_j, position, i, 2, i_problem.GetField(), false);
				}
			}
		}

		return position;
	}
	int Solve_I(IPieceController& i_controller, const Problem& i_problem)
	{
		int position = -1;
		int min_j = i_problem.GetField().GetHeight();

		for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
		{
			if (i_problem.GetField().IsCellFree(i, 0))
			{
				if (field_utils::free_collumn(i, 0, i_problem.GetField()))
				{
					position = i;
					break;
				}
				else
				{
					check_min_row(min_j, position, i, 1, i_problem.GetField(), true);
				}
			}
			else if (i + 1 < i_problem.GetField().GetWidth())
			{
				check_min_row(min_j, position, i, 1, i_problem.GetField(), true);
			}
		}

		int min_level_x = i_problem.GetField().GetWidth();
		int min_level_y = i_problem.GetField().GetHeight();
		for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
		{
			if (min_level_y > field_utils::first_free_row(i, i_problem.GetField()))
			{
				min_level_x = i;
				min_level_y = field_utils::first_free_row(i, i_problem.GetField());
			}
		}

		int pos_j = position != -1 ? field_utils::first_free_row(position, i_problem.GetField()) : 0;
		if (position == -1 || (min_level_y < pos_j && pos_j > 10))
		{
			min_j = i_problem.GetField().GetHeight();
			for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
			{
				if (i < i_problem.GetField().GetWidth())
				{
					check_min_row(min_j, position, i, 1, i_problem.GetField(), false);
				}
			}
		}

		return position;;
	}
	int Solve_J(IPieceController& i_controller, const Problem& i_problem)
	{
		int position = -1;

		int min_j = i_problem.GetField().GetHeight();
		for (size_t i = 0; i < i_problem.GetField().GetWidth(); ++i)
		{
			if ((i + 1) < i_problem.GetField().GetWidth()
				&& i_problem.GetField().IsCellFree(i, 0)
				&& i_problem.GetField().IsCellFree(i + 1, 0))
			{
				if (field_utils::free_collumn(i, 0, i_problem.GetField())
					&& field_utils::free_collumn(i + 1, 0, i_problem.GetField()))
				{
					position = i;
					break;
				}
				else
				{
					check_min_row(min_j, position, i, 2, i_problem.GetField(), true);
				}
			}
			else if (i + 1 < i_problem.GetField().GetWidth())
			{
				check_min_row(min_j, position, i, 2, i_problem.GetField(), true);
			}
		}

		return position;
	}

	void Solve(IPieceController& i_controller, const Problem& i_problem)
	{
		int position = 0;
		switch (i_problem.GetPieceType())
		{
			case PieceType::O:
				position = Solve_O(i_controller, i_problem);
				break;
			case PieceType::I:
				position = Solve_I(i_controller, i_problem);
				break;
			case PieceType::J:
				position = Solve_O(i_controller, i_problem);
				break;
		}

		if (position != -1)
		{

			int shift = i_problem.GetPieceX() - position;

			if (shift > 0)
			{
				for (int i = 0; i < shift; ++i)
					i_controller.Left();
			}
			else
			{
				shift *= -1;
				for (int i = 0; i < shift; ++i)
					i_controller.Right();
			}
		}
		i_controller.Drop();
	}
}

namespace FourthLevel
{
	void Solve(IPieceController& i_controller, const Problem& i_problem)
	{
		std::unique_ptr<Figure> solver = _GetFigureSolver(i_problem.GetPieceType(), i_problem.GetField());

		if (solver == nullptr)
			return;

		int x_pos = solver->GetXPos();

		int shift = i_problem.GetPieceX() - x_pos;

		if (shift > 0)
		{
			for (int i = 0; i < shift; ++i)
				i_controller.Left();
		}
		else
		{
			shift *= -1;
			for (int i = 0; i < shift; ++i)
				i_controller.Right();
		}

		i_controller.Drop();
	}
}


SolverImplementation::~SolverImplementation()
{}

void SolverImplementation::Solve(IPieceController& i_controller, const Problem& i_problem)
{
	switch (i_problem.level)
	{
		case 0:
			FirstLevel::Solve(i_controller, i_problem);
			return;
		case 1:
			SecondLevel::Solve(i_controller, i_problem);
			return;
		case 2:
			ThirdLevel::Solve(i_controller, i_problem);
			return;
	}
}