export module StateLinks;
import <set>;
import State;

export namespace DanceCommon
{
	export template<size_t rowSize>
	class StateLinks
	{
		std::set<State<rowSize>> linksTo;
		std::set<State<rowSize>> linksFrom;
	};
}