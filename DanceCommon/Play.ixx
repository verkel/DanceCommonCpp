export module Play;
import StdCore;
import State;
import NotePos;

namespace DanceCommon
{
	export template<size_t rowSize>
	class Play
	{
		using TState = State<rowSize>;

		map<NotePos, TState> states;

	public:
		size_t GetCount()
		{
			return states.size();
		}

		const TState& operator[](NotePos pos) const
		{
			return states.at(pos);
		}

		TState& operator[](NotePos pos)
		{
			return states.at(pos);
		}
	};
}
