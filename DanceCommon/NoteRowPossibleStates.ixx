export module NoteRowPossibleStates;
import <vector>;
import <map>;
import <unordered_map>;
import <memory>;
import State;
import StateLinks;
import NotePos;
import NoteRow;
import Chart;

namespace DanceCommon
{
	export template<size_t rowSize>
	class NoteRowPossibleStates
	{
		using TState = State<rowSize>;
		using TStateLinks = StateLinks<rowSize>;

	private:
		std::vector<TState> states;
		std::unordered_map<TState, TStateLinks> statesToLinks;
		std::shared_ptr<std::map<NotePos, NoteRowPossibleStates>> parent;

		NotePos previousPosition;
		NotePos position;
		NoteRow<rowSize> noteRow;
		bool allowDoublesteps;

	public:
		NoteRowPossibleStates(NotePos position, NotePos previousPosition, std::shared_ptr<std::map<NotePos, NoteRowPossibleStates>> parent,
			const Chart<rowSize>& chart, bool allowDoublesteps);

		void InsertPossibleStates()
		{
			if (previousPosition == -1)
			{
				//InsertPossibleStatesFor();
			}
		}

		void InsertPossibleStatesFor(const TState& previousState, const TStateLinks& previousStateLinks)
		{
		}

		const std::vector<TState>& GetStates() const
		{
			return states;
		}
	};

	template<size_t rowSize>
	NoteRowPossibleStates<rowSize>::NoteRowPossibleStates(NotePos position, NotePos previousPosition, std::shared_ptr<std::map<NotePos, NoteRowPossibleStates>> parent,
			const Chart<rowSize>& chart, bool allowDoublesteps) :
		states{},
		statesToLinks{},
		parent{parent},
		previousPosition{previousPosition},
		position{position},
		allowDoublesteps{allowDoublesteps}
	{
		noteRow = chart.GetNoteRow(position);
	}
}