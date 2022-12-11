export module State;

import <ostream>;

namespace DanceCommon
{
	export class State
	{
	public:
		friend std::ostream& operator<<(std::ostream& os, const State& state);
	};

	export std::ostream& operator<<(std::ostream& os, const State& state)
	{
		os << "State " << "foo" << "bar";
        return os;
	}
}