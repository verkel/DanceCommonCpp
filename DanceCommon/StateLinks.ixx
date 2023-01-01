export module StateLinks;
import StdCore;
import State;

namespace DanceCommon
{
	export template<size_t rowSize>
	class StateLinks
	{
		using TState = State<rowSize>;

	public:
		StateLinks() = default;
		StateLinks(const StateLinks&) = delete;
		StateLinks& operator=(const StateLinks&) = delete;

		set<TState> linksTo;
		set<TState> linksFrom;
		int costToGoal = -1;
	};
}