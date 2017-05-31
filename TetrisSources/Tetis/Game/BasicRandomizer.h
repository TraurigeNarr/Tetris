#ifndef __BASICRANDOMIZER_H__
#define __BASICRANDOMIZER_H__

#include "IRandomizer.h"

namespace utils
{

	namespace chooser_impl
	{
		struct Probability
		{
			int begin = 0;
			int end = 0;
			Probability(int i_begin, int i_end)
				: begin(i_begin)
				, end(i_end)
			{}
		};
		/*!
		Выбирает случайную вероятность из списка @i_probabilities и возвращает её позицию в списке
		*/
		struct random_policy
		{
			const int Generate(const std::vector<utils::chooser_impl::Probability>& i_probabilities, const int i_probabilities_sum) const
			{
				const int chance = rand() % i_probabilities_sum + 1;
				for (int i = 0; i < static_cast<int>(i_probabilities.size()); ++i)
				{
					if (i_probabilities[i].begin <= chance && chance < i_probabilities[i].end)
						return i;
				}
				return 0;
			}
		};

		/*!
		Выбирает случайную вероятность из списка @i_probabilities и возвращает её позицию в списке.
		Позиции не повторяются, пока не будут перебраны все вхождения
		Приравнивает вероятности выпадения даже если они не равны.
		*/
		struct random_no_repeat_policy
		{
			static constexpr int STEP_ERROR = -1;

			mutable std::vector<int> _used;
			mutable int _previous = STEP_ERROR;

			int GenerateStep(const std::vector<utils::chooser_impl::Probability>& i_probabilities, const int i_probabilities_sum) const
			{
				const int chance = rand() % i_probabilities_sum + 1;
				for (int i = 0; i < static_cast<int>(i_probabilities.size()); ++i)
				{
					if (i_probabilities[i].begin <= chance && chance < i_probabilities[i].end)
						return i;
				}
				return STEP_ERROR;
			}

			const int Generate(const std::vector<utils::chooser_impl::Probability>& i_probabilities, const int i_probabilities_sum) const
			{
				static constexpr int TRYES_COUNT = 5;
				int choosed = STEP_ERROR;
				for (int i = 0; i < TRYES_COUNT; ++i)
				{
					const int choosed_step = GenerateStep(i_probabilities, i_probabilities_sum);

					if (std::find(_used.begin(), _used.end(), choosed_step) != _used.end())
						continue;

					if (choosed_step == _previous)
						continue;

					if (choosed_step != STEP_ERROR)
					{
						choosed = choosed_step;
						break;
					}
				}
				// Ошибка при недостаточности попыток
				if (choosed == STEP_ERROR)
				{
					for (int i = 0; i < static_cast<int>(i_probabilities.size()); ++i)
					{
						if (std::find(_used.begin(), _used.end(), i) == _used.end())
						{
							choosed = i;
							break;
						}
					}
				}

				_previous = STEP_ERROR;
				_used.push_back(choosed);
				if (_used.size() == i_probabilities.size())
				{
					_used.clear();
					_previous = choosed;
				}
				return choosed;
			}
		};

	} // chooser_impl

	  /*!
	  Позволяет с нужной вероятностью выдавать одно из сохранённых событий
	  Событием считается пользовательский тип данных - может быть строка или что-то своё
	  Вероятность задаётся целочисленным типом (int) и является частью от суммы всех вероятностей
	  сохранённых событий. Если сумма не будет 100, то число-вероятность не будет равнятся реальному
	  шансу выпадения данного события
	  */
	template <typename T, typename ChooserPolicy = chooser_impl::random_policy>
	class RandomChooser
	{
	private:
		std::vector<chooser_impl::Probability> _probabilities;
		std::vector<T> _events;

		// Пара событие-вероятность
		int _probabilitySums = 0;

		ChooserPolicy _chooser;

	public:
		/// Очистка событий
		void Flush()
		{
			_events.clear();
			_probabilities.clear();
			_probabilitySums = 0;
		}

		/// Добавление события с вероятностью @i_probability
		void AddEvent(const T& i_event, int i_probability)
		{
			_events.emplace_back(i_event);
			_probabilities.emplace_back(chooser_impl::Probability{ _probabilitySums, _probabilitySums + i_probability });

			_probabilitySums += i_probability;
		}

		///	Возвращает согласно заданным вероятностям событие
		/// Если вектор событий пустой возвращает nullptr
		const T* GenerateEvent() const
		{
			if (_events.empty())
				return nullptr;

			int event_num = _chooser.Generate(_probabilities, _probabilitySums);
			if (event_num >= static_cast<int>(_events.size()))
				return nullptr;
			return &_events[event_num];
		}
	};


} // utils

class BasicRandomizer : public IRandomizer
{
private:
	utils::RandomChooser<int> m_random;

public:
	BasicRandomizer();
	~BasicRandomizer();

	virtual std::unique_ptr<TetrisPiece> GetNext(IField& i_field) const override;
	virtual void SetParameters(RandomizerParameters i_parameters) override;
};

#endif