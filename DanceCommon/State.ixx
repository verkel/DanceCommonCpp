export module State;

import LimbsOnPad;
import Limb;
import PlayStyle;
import NoteRow;
import Panel;
import FeetPlacement;
import Computations;
import StringUtils;
import FacingType;
import <ostream>;
import <stdexcept>;
import <sstream>;
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
			int cost = 0;

			Panel leftLegPanel = GetOccupyingPanel(Limb::LeftLeg);
			Panel rightLegPanel = GetOccupyingPanel(Limb::RightLeg);
			Panel leftHandPanel = GetOccupyingPanel(Limb::LeftHand);
			Panel rightHandPanel = GetOccupyingPanel(Limb::RightHand);

			FeetPlacement placement = Computations::GetFeetPlacement(leftLegPanel, rightLegPanel);
			FacingType facingType = placement.GetFacingType();

			int angle = abs(GetAngle());
			int angleDelta = abs(GetAngleDelta());

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
				default: throw std::invalid_argument("doublestepLeg");
			}
		}

		int GetNumberOfFreeLegs() const
		{
			int amount = 0;
			if (Limbs::Contains(freeLimbs, Limb::LeftLeg)) amount++;
			if (Limbs::Contains(freeLimbs, Limb::RightLeg)) amount++;
			return amount;
		}
		
		std::string ToString() const
		{
			std::stringstream ss;
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

			int angleDelta = angle - previousAngle;
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
		Append(os, state, true);
        return os;
	}

	export template<size_t rowSize>
	std::ostream& Append(std::ostream& os, const State<rowSize>& state, bool displayClassName)
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
	std::ostream& AppendBody(std::ostream& os, const State<rowSize>& state)
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
