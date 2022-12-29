export module StateLinks;
import StdCore;
import State;

export namespace DanceCommon
{
	export template<size_t rowSize>
	class StateLinks
	{
	public:
		set<shared_ptr<State<rowSize>>> linksTo;
		set<shared_ptr<State<rowSize>>> linksFrom;
	};
}