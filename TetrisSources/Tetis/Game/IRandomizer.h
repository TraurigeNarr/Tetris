#ifndef __IRANDOMIZER_H__
#define __IRANDOMIZER_H__

class TetrisPiece;
struct IField;
enum class PieceType;

struct TypeParameters
{
	PieceType		m_piece;
	unsigned int	m_color;
	int				m_probability;

	TypeParameters(PieceType i_type, int i_probability, unsigned int i_color)
		: m_piece(i_type)
		, m_probability(i_probability)
		, m_color(i_color)
	{}
};

using RandomizerParameters = std::vector<TypeParameters>;

class IRandomizer
{
protected:
	RandomizerParameters m_parameters;
	
public:
	virtual ~IRandomizer() {}

	virtual void SetParameters(RandomizerParameters i_parameters) { m_parameters = i_parameters; }
	virtual std::unique_ptr<TetrisPiece> GetNext(IField& i_field) const = 0;
	virtual std::vector<PieceType> GetNextPieces() const { return std::vector<PieceType>(); }
};

#endif