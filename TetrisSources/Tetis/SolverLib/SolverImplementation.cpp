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



SolverImplementation::~SolverImplementation()
{}

void SolverImplementation::Solve(IPieceController& i_controller, const Problem& i_problem)
{
	std::unique_ptr<Figure> solver = _GetFigureSolver(i_problem.GetPieceType(), i_problem.GetField());

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