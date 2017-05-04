#ifndef __IRANDOMIZER_H__
#define __IRANDOMIZER_H__

class TetrisPiece;
struct IField;
enum class PieceType;

struct RandomizerParameters
{
	struct TypeParameters
	{
		PieceType		m_piece;
		unsigned int	m_color;
		bool			m_rotate;

		TypeParameters(PieceType i_type, bool i_rotate, unsigned int i_color)
			: m_piece(i_type)
			, m_rotate(i_rotate)
			, m_color(i_color)
		{}
	};
	std::vector<TypeParameters> m_type_for_generation;
};

class IRandomizer
	{
	protected:
		RandomizerParameters m_parameters;

	public:
		virtual ~IRandomizer(){}

		const RandomizerParameters& GetParameters() const { return m_parameters; }
		void SetParameters(const RandomizerParameters& i_parameters) { m_parameters = i_parameters; }
		virtual std::unique_ptr<TetrisPiece> GetNext(IField& i_field) const = 0;
	};

#endif