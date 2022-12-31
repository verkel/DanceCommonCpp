export module State;

import StdCore;
import LimbsOnPad;
import Limb;
import PlayStyle;
import NoteRow;
import Panel;
import FeetPlacement;
import Computations;
import StringUtils;
import FacingType;
import <cstdlib>;

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
		unsigned char movedLegsAmount;
		short angleDelta;
		short cost;
		bool leftLegFreed, rightLegFreed;
		bool doublestep;
		bool airDoublestep;
		bool spin;

	public:
		//friend ostream& operator<<(ostream& os, const State<rowSize>& state);
		friend std::hash<State>;

		constexpr State() :
			State(TLimbsOnPad{}, Limb::None, Limbs::All)
		{
		}

		constexpr State(const TLimbsOnPad& occupiedPanels, Limb lastLeg, Limb freeLimbs) :
			State(occupiedPanels, lastLeg, freeLimbs, false, false, 0, 0, false, false, false)
		{
		}

		constexpr State(const TLimbsOnPad& occupiedPanels, Limb lastLeg, Limb freeLimbs, bool doublestep, bool airDoublestep,
				unsigned char movedLegsAmount, short angleDelta, bool spin, bool leftLegFreed, bool rightLegFreed) :
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

		bool operator<(const State& rhs) const
		{
			if (cost != rhs.cost)
				return cost < rhs.cost;

			if (doublestep != rhs.doublestep)
				return doublestep < rhs.doublestep;

			if (airDoublestep != rhs.airDoublestep)
				return airDoublestep < rhs.airDoublestep;

			if (spin != rhs.spin)
				return spin < rhs.spin;

			if (angleDelta != rhs.angleDelta)
				return angleDelta < rhs.angleDelta;

			if (movedLegsAmount != rhs.movedLegsAmount)
				return movedLegsAmount < rhs.movedLegsAmount;

			if (freeLimbs != rhs.freeLimbs)
				return freeLimbs < rhs.freeLimbs;

			if (occupiedPanels != rhs.occupiedPanels)
				return occupiedPanels < rhs.occupiedPanels;

			return lastLeg < rhs.lastLeg;
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

			auto [tappables, tappablesCount] = noteRow.GetTappablesWithCount();
			auto firstTappable = noteRow.GetFirstTappable();
			//auto holdables = noteRow.GetHoldables();

			UpdateLastLeg(newState, limbsUsed, tappablesCount, firstTappable);
			CheckDoublesteps(newState, limbsUsed, noteRow, tappablesCount, firstTappable);

			newState.leftLegFreed = false;
			newState.rightLegFreed = false;

			UpdateOccupiedPanels(newState, limbsUsed);
			ReserveLimbs(newState, limbsUsed, noteRow);
			ComputeMovedLegs(newState);
			ComputeAngleDeltaAndSpin(newState);

			return newState;
		}

		int ComputeCost() const
		{
			short cost = 0;

			Panel leftLegPanel = GetOccupyingPanel(Limb::LeftLeg);
			Panel rightLegPanel = GetOccupyingPanel(Limb::RightLeg);
			Panel leftHandPanel = GetOccupyingPanel(Limb::LeftHand);
			Panel rightHandPanel = GetOccupyingPanel(Limb::RightHand);

			FeetPlacement placement = Computations::GetFeetPlacement(leftLegPanel, rightLegPanel);
			FacingType facingType = placement.GetFacingType();

			int angle = abs(GetAngle());
			short angleDelta = abs(GetAngleDelta());

			if (angle >= 135) cost += 1;

			if (angleDelta == 135) cost += 5;
			else if (angleDelta == 180) cost += 15;

			if (doublestep) cost += 12;
			else if (airDoublestep) cost += 10; // Prefer ADs slightly to dsteps
			else if (spin) cost += 3; // If we can spin cleanly, that's most preferred

			// Tax hand use based on FacingType
			if (Panels::IsButton(leftHandPanel)) {
				cost += GetFacingCost(facingType);
			}
			if (Panels::IsButton(rightHandPanel)) {
				cost += GetFacingCost(facingType);
			}

			// If hands are in wrong orientation, tax this state
			bool lhWronglyPlaced = (Panels::IsButton(leftHandPanel) && leftHandPanel != placement.GetLeftHandPanel());
			bool rhWronglyPlaced = (Panels::IsButton(rightHandPanel) && rightHandPanel != placement.GetRightHandPanel());
			if (lhWronglyPlaced || rhWronglyPlaced) {
				cost += 5;
			}

			// If hands are used for holdables, tax this state
			// Note, this accumulates in every tap during hand holds
			// Might be better to tax just beginning a hand hold, but
			// starting holds is not currently tracked by the state.
			if (!IsFree(Limb::LeftHand)) cost += 1;
			if (!IsFree(Limb::RightHand)) cost += 1;

			return cost;
		}

		static int GetFacingCost(FacingType facingType)
		{
			switch (facingType)
			{
				case FacingType::FaceIn: return 0;
				case FacingType::FaceOut: return 2;
				case FacingType::FaceToSide: return 1;
				default: return 0;
			}
		}

		const TLimbsOnPad& GetOccupiedPanels() const
		{
			return occupiedPanels;
		}

		bool IsFree(Limb limb) const
		{
			return Limbs::Contains(freeLimbs, limb);
		}
		
		Limb GetNextFreeLeg() const
		{
			Limb leg = Limbs::GetPair(lastLeg);
			if (leg == Limb::None) return Limb::None;

			if (IsFree(leg)) return leg;

			Limb pair = Limbs::GetPair(leg);
			if (IsFree(pair)) return pair;

			return Limb::None;
		}
		
		Panel GetOccupyingPanel(Limb limb) const
		{
			return occupiedPanels.GetOccupyingPanel(limb);
		}

		Limb GetFreeLimbs() const
		{
			return freeLimbs;
		}

		Limb GetLastLeg() const
		{
			return lastLeg;
		}
		
		int GetAngleDelta() const
		{
			return angleDelta;
		}

		int GetAngle() const
		{
			Panel leftLegPanel = GetOccupyingPanel(Limb::LeftLeg);
			Panel rightLegPanel = GetOccupyingPanel(Limb::RightLeg);

			FeetPlacement placement = Computations::GetFeetPlacement(leftLegPanel, rightLegPanel);
			return placement.GetAngle();
		}

		int GetMovedLegsAmount() const
		{
			return movedLegsAmount;
		}

		int GetCost()
		{
			if (cost == -1)
				cost = ComputeCost();

			return cost;
		}

		bool IsDoublestep() const
		{
			return doublestep;
		}

		bool IsAirDoublestep() const
		{
			return airDoublestep;
		}

		bool IsSpin() const
		{
			return spin;
		}

		bool IsLeftLegFreed() const
		{
			return leftLegFreed;
		}

		bool IsRightLegFreed() const
		{
			return rightLegFreed;
		}

		static bool IsDoublestepEnabled(Limb doublestepLeg, State state)
		{
			switch (doublestepLeg)
			{
				case Limb::LeftLeg: return !state.rightLegFreed;
				case Limb::RightLeg: return !state.leftLegFreed;
				default: throw invalid_argument("doublestepLeg");
			}
		}

		int GetNumberOfFreeLegs() const
		{
			int amount = 0;
			if (Limbs::Contains(freeLimbs, Limb::LeftLeg)) amount++;
			if (Limbs::Contains(freeLimbs, Limb::RightLeg)) amount++;
			return amount;
		}
		
		string ToString() const
		{
			stringstream ss;
			ss << *this;
			return ss.str();
		}

	private:
		static void UpdateLastLeg(State& state, const TLimbsOnPad& limbsUsed, int tappablesCount, Panel firstTappable)
		{
			if (tappablesCount > 0) state.lastLeg = Limb::None;

			if (tappablesCount == 1 && state.GetNumberOfFreeLegs() > 0)
			{
				Limb limbAtTappable = limbsUsed[firstTappable];
				if (Limbs::IsLeg(limbAtTappable))
				{
					state.lastLeg = limbAtTappable;
				}
			}
		}

		void CheckDoublesteps(State& state, const TLimbsOnPad& limbsUsed, const TNoteRow& noteRow, int tappablesCount, Panel firstTappable) const
		{
			state.doublestep = false;
			state.airDoublestep = false;

			if (state.GetNumberOfFreeLegs() == 2)
			{
				// Check if it's a normal doublestep
				if (tappablesCount == 1)
				{
					if (this->lastLeg == state.lastLeg && IsDoublestepEnabled(state.lastLeg, state))
					{
						if (this->GetOccupyingPanel(this->lastLeg) != firstTappable)
						{
							state.doublestep = true;
						}
					}
				}

				// Check if it's an "air doublestep"
				else if (tappablesCount > 1)
				{
					noteRow.ForEachTappable([&](Panel panel)
					{
						const Limb occupyingLimb = this->occupiedPanels[panel];
						if (Limbs::IsLeg(occupyingLimb))
						{
							const Limb limbToUse = limbsUsed[panel];
							if (occupyingLimb != limbToUse)
							{
								state.airDoublestep = true;
							}
						}
					});
				}
			}
		}

		static void UpdateOccupiedPanels(State& state, const TLimbsOnPad& limbsUsed)
		{
			// Remove all limbs from their old positions
			for (int i = TLimbsOnPad::First(); i < TLimbsOnPad::End(); i++)
			{
				Limb usedLimb = limbsUsed[i];
				if (usedLimb == Limb::None) continue;

				state.occupiedPanels.RemoveLimb(usedLimb);
			}

			// Put the limbs back into their new positions
			for (int i = TLimbsOnPad::First(); i < TLimbsOnPad::End(); i++)
			{
				Limb usedLimb = limbsUsed[i];
				if (usedLimb == Limb::None) continue;

				state.occupiedPanels.PlaceLimb(Panels::ForIndex(i), usedLimb);
			}
		}

		static void ReserveLimbs(State& state, const TLimbsOnPad& limbsUsed, const TNoteRow& noteRow)
		{
			noteRow.ForEachHoldable([&](Panel panel)
			{
				Limb limbAtHoldable = limbsUsed[panel];
				if (limbAtHoldable != Limb::None)
				{
					state.ReserveLimb(limbAtHoldable);
				}
			});
		}

		void ComputeMovedLegs(State& state) const
		{
			state.movedLegsAmount = 0;
			for (int i = state.occupiedPanels.First(); i < state.occupiedPanels.End(); i++)
			{
				Limb limb = state.occupiedPanels[i];
				if (!Limbs::IsLeg(limb)) continue;
				if (limb != this->occupiedPanels[i]) state.movedLegsAmount++;
			}
		}

		void ComputeAngleDeltaAndSpin(State& state) const
		{
			int previousAngle = this->GetAngle();
			int angle = state.GetAngle();

			short angleDelta = angle - previousAngle;
			int properAngleDelta = angleDelta + ((angleDelta > 180) ? -360 : (angleDelta < -180) ? 360 : 0);

			state.angleDelta = properAngleDelta;

			// Determine if a spin
			// Cases around 180 need to be handled specifically
			if (angle == 180) state.spin = false; // Not a spin (at least yet)
			else if (previousAngle == 180) // Is a spin if we don't back out
			{
				int previousAngleDelta = this->GetAngleDelta();
				if (previousAngleDelta > 0 && properAngleDelta > 0) state.spin = true;
				else if (previousAngleDelta < 0 && properAngleDelta < 0) state.spin = true;
				else state.spin = false;
			}
			// It's a spin if the angleDelta needed fixing
			else state.spin = (angleDelta != properAngleDelta);
		}

		static void ReleaseHoldEnds(State& state, const TNoteRow& noteRow)
		{
			state.leftLegFreed = false;
			state.rightLegFreed = false;

			noteRow.ForEachHoldEnd([&](Panel holdEndPanel)
			{
				Limb holdingLimb = state.occupiedPanels[holdEndPanel];
				if (holdingLimb == Limb::None) throw logic_error("State did not have a limb on a hold");

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
	ostream& operator<<(ostream& os, const State<rowSize>& state)
	{
		Append(os, state, true);
        return os;
	}

	export template<size_t rowSize>
	ostream& Append(ostream& os, const State<rowSize>& state, bool displayClassName)
	{
		if (displayClassName)
			os << "State { ";

		AppendBody(os, state);

		os << ", cost=" << state.ComputeCost()
			<< ", angle=" << state.GetAngle()
			<< ", dAngle=" << state.GetAngleDelta()
			<< ", dstep=" << StringUtils::ToString(state.IsDoublestep())
			<< ", airDstep=" << StringUtils::ToString(state.IsAirDoublestep())
			<< ", spin=" << StringUtils::ToString(state.IsSpin())
			<< ", movedLegs=" << state.GetMovedLegsAmount()
			<< ", llFreed=" << StringUtils::ToString(state.IsLeftLegFreed())
			<< ", rlFreed=" << StringUtils::ToString(state.IsRightLegFreed());

		if (displayClassName)
			os << " }";

		return os;
	}

	export template<size_t rowSize>
	ostream& AppendBody(ostream& os, const State<rowSize>& state)
	{
		auto occupiedPanels = state.GetOccupiedPanels();
		auto lastLeg = state.GetLastLeg();

		// Append single center panel if something is on it
		Limb limbOnCenter = occupiedPanels[Panel::Center];
		if (limbOnCenter != Limb::None) os << "(" << Limbs::GetCompactName(limbOnCenter) << ") ";

		// Append rest of the panels
		for (int i = 0; i < occupiedPanels.End(); i++)
		{
			Limb limb = occupiedPanels[i];
			if (limb != Limb::None)
			{
				os << (state.IsFree(limb)
					? "["
					: "{");
				
				os << Limbs::GetCompactName(limb);

				os << (lastLeg == limb
					? "*"
					: Limbs::IsLeg(limb)
						? " "
						: "");

				os << (state.IsFree(limb)
					? "]"
					: "}");
			}
			else
			{
				os << "[  ]";
			}

			if (i != occupiedPanels.End() - 1) os << " ";
			if (i == 3 && PlayStyles::GetStyle(rowSize) == PlayStyle::Double)
			{
				os << "- ";
				Limb limbOnCenter2 = occupiedPanels[Panel::Center2];
				if (limbOnCenter2 != Limb::None) os << "(" << Limbs::GetCompactName(limbOnCenter2) << ") ";
			}
		}

        return os;
	}

	export template<size_t rowSize>
	class States
	{
	private:
		// I'm seeing some memory corruption happening with this. freeLimbs = 88 instead of 15=Limbs:All
		// and TestNoteRowPossibleStates randomly failing because of this.
		static inline constexpr State<rowSize> Empty = State{LimbsOnPad<rowSize>{}, Limb::None, Limbs::All};

	public:
		static const State<rowSize>& GetDefault();

		static const State<rowSize>& GetEmpty()
		{
			return Empty;
		}
	};

	static inline constexpr State DefaultStateSingle{LimbsOnPad<NoteRowSize::Single>{
		Limb::None, Limb::LeftLeg, Limb::None, Limb::None, Limb::RightLeg},
		Limb::None, Limbs::All };

	export template<> // specialization for NoteRowSize::Single
	const State<NoteRowSize::Single>& States<NoteRowSize::Single>::GetDefault()
	{
		return DefaultStateSingle;
	}

	static inline constexpr State DefaultStateDouble{LimbsOnPad<NoteRowSize::Double>{
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
		size_t operator()(const DanceCommon::State<rowSize>& s) const
		{
			// TODO implement
			//return 0;

			/*using size_t;
				using hash;
				using string;

				// Compute individual hash values for first,
				// second and third and combine them using XOR
				// and bit shifting:

				return ((hash<string>()(k.first)
					^ (hash<string>()(k.second) << 1)) >> 1)
					^ (hash<int>()(k.third) << 1);*/

			constexpr int prime = 31;
			int result = 1;
			result = prime * result + s.angleDelta;
			result = prime * result + (s.doublestep ? 1231 : 1237);
			result = prime * result + (s.airDoublestep ? 1231 : 1237);
			result = prime * result + static_cast<int>(s.freeLimbs);
			result = prime * result + static_cast<int>(s.lastLeg);
			result = prime * result + (s.leftLegFreed ? 1231 : 1237);
			result = prime * result + s.movedLegsAmount;
			result = prime * result + s.occupiedPanels.GetHashCode();
			result = prime * result + (s.rightLegFreed ? 1231 : 1237);
			result = prime * result + (s.spin ? 1231 : 1237);
			return result;
		}
	};
}
