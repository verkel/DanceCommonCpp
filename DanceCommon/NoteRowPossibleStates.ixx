export module NoteRowPossibleStates;
import <vector>;
import <unordered_map>;
import State;
import StateLinks;
import NotePos;
import NoteRow;

namespace DanceCommon
{
	export template<size_t rowSize>
	class NoteRowPossibleStates
	{
	private:
		std::vector<State> states;
		std::unordered_map<State, StateLinks> statesToLinks;

		NotePos previousPosition;
		NotePos position;
		NoteRow<rowSize> noteRow;
	};
}