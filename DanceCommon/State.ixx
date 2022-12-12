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

template <>
struct std::hash<DanceCommon::State>
{
	std::size_t operator()(const DanceCommon::State& k) const
	{
		// TODO implement
		return 0;

		/*using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<string>()(k.first)
				^ (hash<string>()(k.second) << 1)) >> 1)
				^ (hash<int>()(k.third) << 1);*/
	}
};
