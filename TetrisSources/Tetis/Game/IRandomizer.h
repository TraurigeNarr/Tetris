#ifndef __IRANDOMIZER_H__
#define __IRANDOMIZER_H__

class TetrisPiece;
struct IField;
class ISolver;

struct RandomizerParameters
	{

	};

class IRandomizer
	{
	private:
		RandomizerParameters m_parameters;

	public:
		virtual ~IRandomizer(){}

		const RandomizerParameters& GetParameters() const { return m_parameters; }
		void SetParameters(const RandomizerParameters& i_parameters) { m_parameters = i_parameters; }
		virtual std::unique_ptr<TetrisPiece> GetNext(IField& i_field, ISolver* ip_solver) const = 0;
	};

#endif