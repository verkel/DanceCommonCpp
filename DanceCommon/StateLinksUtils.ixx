export module StateLinksUtils;

import StdCore;
import NoteRowPossibleStates;
import StateLinks;
import State;

namespace DanceCommon
{
	// These could be methods on StateLinks, but they need to be on StateLinks.cpp to allow circular dependency with
	// NoteRowPossibleStates and then unit tests can't find StateLinks.obj in linking phase
	// (even if I add it with Linker->Input->Additional Dependencies)
	export template<size_t rowSize>
	class StateLinksUtils
	{
		using TNoteRowPossibleStates = NoteRowPossibleStates<rowSize>;
		using TStateLinks = StateLinks<rowSize>;
		using TState = State<rowSize>;

	public:
		static int GetCheapestChildCost(const TStateLinks& stateLinks, const TNoteRowPossibleStates& childStates)
		{
			if (stateLinks.linksFrom.size() == 0) return 0;

			int lowestCost = INT_MAX;
			for (const TState& state : stateLinks.linksFrom)
			{
				int cost = childStates.GetStateLinks(state).costToGoal;
				if (cost < lowestCost) lowestCost = cost;
			}

			return lowestCost;
		}

		static const TState& GetCheapestChild(const TStateLinks& stateLinks, const TNoteRowPossibleStates& childStates)
		{
			int lowestCost = INT_MAX;
			optional<reference_wrapper<const TState>> lowestChild = nullopt;
			for (const TState& state : stateLinks.linksFrom)
			{
				int cost = childStates.GetStateLinks(state).costToGoal;
				if (cost < lowestCost) 
				{
					lowestCost = cost;
					lowestChild = state;
				}
			}

			if (lowestChild)
				return lowestChild.value();
			else
				throw range_error("no children");
		}
	};
}