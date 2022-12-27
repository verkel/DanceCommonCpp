export module State;

import LimbsOnPad;
import Limb;
import PlayStyle;
import NoteRow;
import Panel;
import <ostream>;
import <stdexcept>;

namespace DanceCommon
{
	export template<size_t rowSize>
	class State
	{
		using TNoteRow = NoteRow<rowSize>;
		using TLimbsOnPad = LimbsOnPad<rowSize>;

	private:
		TLimbsOnPad occupiedPanels;
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

		friend bool operator==(const State& lhs, const State& rhs)
		{
			return lhs.occupiedPanels == rhs.occupiedPanels
				&& lhs.lastLeg == rhs.lastLeg
				&& lhs.freeLimbs == rhs.freeLimbs
				&& lhs.movedLegsAmount == rhs.movedLegsAmount
				&& lhs.angleDelta == rhs.angleDelta
				&& lhs.cost == rhs.cost
				&& lhs.leftLegFreed == rhs.leftLegFreed
				&& lhs.rightLegFreed == rhs.rightLegFreed
				&& lhs.doublestep == rhs.doublestep
				&& lhs.airDoublestep == rhs.airDoublestep
				&& lhs.spin == rhs.spin;
		}

		friend bool operator!=(const State& lhs, const State& rhs)
		{
			return !(lhs == rhs);
		}

		State Before(const TNoteRow& noteRow) const
		{
			auto newState = *this;
			ReleaseHoldEnds(newState, noteRow);
			UnoccupyHands(newState);
			AvoidMines(newState, noteRow);
			return newState;
		}

		State Tap(const TNoteRow& noteRow, const TLimbsOnPad& limbsUsed) const
		{
			auto newState = *this;

			auto noteRowTappables = noteRow.GetTappables();
			auto noteRowHoldables = noteRow.GetHoldables();

			//UpdateLastLeg(state, limbsUsed, noteRowTappables);
			//CheckDoublesteps(state, limbsUsed, noteRowTappables);

			//state.leftLegFreed = false;
			//state.rightLegFreed = false;

			//UpdateOccupiedPanels(state, limbsUsed);
			//ReserveLimbs(state, limbsUsed, noteRowHoldables);
			//ComputeMovedLegs(state);
			//ComputeAngleDeltaAndSpin(state);

			return newState;
		}

	private:
		static void ReleaseHoldEnds(State& state, const TNoteRow& noteRow)
		{
			state.leftLegFreed = false;
			state.rightLegFreed = false;

			noteRow.ForEachHoldEnd([&](Panel holdEndPanel)
			{
				Limb holdingLimb = state.occupiedPanels[holdEndPanel];
				if (holdingLimb == Limb::None) throw std::logic_error("State did not have a limb on a hold");

				// Hands don't occupy panels after holds, so remove it
				if (Limbs::IsHand(holdingLimb)) state.occupiedPanels[holdEndPanel] = Limb::None;
				state.FreeLimb(holdingLimb);

				// Flag legs that end holds
				if (holdingLimb == Limb::LeftLeg) state.leftLegFreed = true;
				else if (holdingLimb == Limb::RightLeg) state.rightLegFreed = true;
			});
		}

		void UnoccupyHands(State& state) const
		{
			for (int i = TLimbsOnPad::First(); i < TLimbsOnPad::End(); i++)
			{
				Limb limb = state.occupiedPanels[i];
				if (Limbs::IsHand(limb) && IsFree(limb)) state.occupiedPanels[i] = Limb::None;
			}
		}

		static void AvoidMines(State& state, const TNoteRow& noteRow)
		{
			noteRow.ForEachMine([&](Panel minePanel)
			{
				Limb limbAtMine = state.occupiedPanels[minePanel];
				if (Limbs::IsHand(limbAtMine))
				{
					state.occupiedPanels[minePanel] = Limb::None;
				}
				else if (Limbs::IsLeg(limbAtMine))
				{
					Panel nearestCenterPanel = Panel::Center; // TODO calculate nearest center panel in double
					state.occupiedPanels.MoveLimb(nearestCenterPanel, limbAtMine);
					if (state.lastLeg == limbAtMine) state.lastLeg = Limb::None;
				}
			});
		}

		bool IsFree(Limb limb) const
		{
			return Limbs::Contains(freeLimbs, limb);
		}

		void ReserveLimb(Limb limb)
		{
			freeLimbs = Limbs::WithRemoved(freeLimbs, limb);
		}

		void FreeLimb(Limb limb)
		{
			freeLimbs = Limbs::WithAdded(freeLimbs, limb);
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

	export template<size_t rowSize>
	class States
	{
	private:
		static inline const State<rowSize> Empty = State{LimbsOnPad<rowSize>{}, Limb::None, Limbs::All};

	public:
		static const State<rowSize>& GetDefault();

		static const State<rowSize>& GetEmpty()
		{
			return Empty;
		}
	};

	const State<NoteRowSize::Single> DefaultStateSingle{LimbsOnPad<NoteRowSize::Single>{
		Limb::None, Limb::LeftLeg, Limb::None, Limb::None, Limb::RightLeg},
		Limb::None, Limbs::All };

	export template<> // specialization for NoteRowSize::Single
	const State<NoteRowSize::Single>& States<NoteRowSize::Single>::GetDefault()
	{
		return DefaultStateSingle;
	}

	const State<NoteRowSize::Double> DefaultStateDouble{LimbsOnPad<NoteRowSize::Double>{
		Limb::None, Limb::None, Limb::LeftLeg, Limb::None, Limb::None, Limb::RightLeg, Limb::None, Limb::None, Limb::None, Limb::None},
		Limb::None, Limbs::All };

	export template<> // specialization for NoteRowSize::Double
	const State<NoteRowSize::Double>& States<NoteRowSize::Double>::GetDefault()
	{
		return DefaultStateDouble;
	}
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
