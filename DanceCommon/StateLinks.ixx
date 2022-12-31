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
		set<shared_ptr<TState>> linksTo;
		set<shared_ptr<TState>> linksFrom;
		int costToGoal = -1;
	};
}