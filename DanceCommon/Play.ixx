export module Play;
import State;
import NotePos;

namespace DanceCommon
{
	export template<size_t rowSize>
	class Play
	{
		using TState = State<rowSize>;

	public:
		virtual ~Play() = default;
		virtual size_t GetCount() = 0;

		virtual const TState& operator[](NotePos pos) const = 0;
	};
}
