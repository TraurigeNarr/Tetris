#ifndef __IRANDOMIZER_H__
#define __IRANDOMIZER_H__

class TetrisPiece;
struct IField;
class ISolver;
enum class PieceType;

struct RandomizerParameters
	{
	struct TypeParameters
		{
		PieceType m_piece;
		bool			m_rotate;
		TypeParameters(PieceType i_type, bool i_rotate)
			: m_piece(i_type)
			, m_rotate(i_rotate)
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
		virtual std::unique_ptr<TetrisPiece> GetNext(IField& i_field, ISolver* ip_solver) const = 0;
	};

#endif