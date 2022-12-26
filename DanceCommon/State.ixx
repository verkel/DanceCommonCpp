export module State;

import LimbsOnPad;
import Limb;
import PlayStyle;
import <ostream>;

export namespace DanceCommon
{
	export template<size_t rowSize>
	class State
	{
	private:
		LimbsOnPad<rowSize> occupiedPanels;
		Limb lastLeg;
		Limb /*bitmask*/ freeLimbs;
		int movedLegsAmount;
		int angleDelta;
		int cost;
		bool leftLegFreed, rightLegFreed;
		bool doublestep;
		bool airDoublestep;
		bool spin;
	public:
		//friend std::ostream& operator<<(std::ostream& os, const State<rowSize>& state);

		State(const LimbsOnPad<rowSize>& occupiedPanels, Limb lastLeg, Limb freeLimbs) :
			State(occupiedPanels, lastLeg, freeLimbs, false, false, 0, 0, false, false, false)
		{
		}

		State(const LimbsOnPad<rowSize>& occupiedPanels, Limb lastLeg, Limb freeLimbs, bool doublestep,	bool airDoublestep,
				int movedLegsAmount, int angleDelta, bool spin, bool leftLegFreed, bool rightLegFreed) :
			occupiedPanels(occupiedPanels),
			lastLeg(lastLeg),
			freeLimbs(freeLimbs),
			movedLegsAmount(movedLegsAmount),
			angleDelta(angleDelta),
			cost(-1),
			leftLegFreed(leftLegFreed),
			rightLegFreed(rightLegFreed),
			doublestep(doublestep),
			airDoublestep(airDoublestep),
			spin(spin)
		{
		}
	};

	export using SinglesState = State<NoteRowSize::Single>;
	export using DoublesState = State<NoteRowSize::Double>;

	export template<size_t rowSize>
	std::ostream& operator<<(std::ostream& os, const State<rowSize>& state)
	{
		os << "State " << "foo" << "bar";
        return os;
	}

	State<NoteRowSize::Single> DefaultSingle{LimbsOnPad<NoteRowSize::Single>{
		Limb::None, Limb::LeftLeg, Limb::None, Limb::None, Limb::RightLeg},
		Limb::None, Limbs::All };

	export template<size_t rowSize>
	class States
	{
	public:
		static inline const State<rowSize> Empty{LimbsOnPad<rowSize>{}, Limb::None, Limbs::All};
	};
}

namespace std
{
	export template <size_t rowSize>
	struct hash<DanceCommon::State<rowSize>>
	{
		std::size_t operator()(const DanceCommon::State<rowSize>& k) const
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
}
