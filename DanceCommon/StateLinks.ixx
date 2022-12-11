export module StateLinks;
import <set>;
import State;

export namespace DanceCommon
{
	export class StateLinks
	{
		std::set<State> linksTo;
		std::set<State> linksFrom;
	};
}