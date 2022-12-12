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
	private:
		std::vector<State> states;
		std::unordered_map<State, StateLinks> statesToLinks;
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
			// TODO implement
		}

		const std::vector<State>& GetStates() const
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