export module StateLinks;
import <set>;
import <memory>;
import State;

export namespace DanceCommon
{
	export template<size_t rowSize>
	class StateLinks
	{
	public:
		std::set<std::shared_ptr<State<rowSize>>> linksTo;
		std::set<std::shared_ptr<State<rowSize>>> linksFrom;
	};
}