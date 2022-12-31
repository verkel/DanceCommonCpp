export module NoteRowPossibleStates;

import StdCore;
import State;
import StateLinks;
import NotePos;
import NoteRow;
import NoteType;
import Chart;
import Computations;
import FeetPlacement;
import LimbsOnPad;
import Limb;
import Panel;

namespace DanceCommon
{
	export template<size_t rowSize>
	class NoteRowPossibleStates
	{
		using TState = State<rowSize>;
		using TStates = States<rowSize>;
		using TStateLinks = StateLinks<rowSize>;
		using TLimbsOnPad = LimbsOnPad<rowSize>;
		using TLimbsOnPads = LimbsOnPads<rowSize>;
		using TOptionalStateLinks = optional<reference_wrapper<TStateLinks>>;

	private:
		vector<shared_ptr<TState>> states;
		unordered_map<TState, TStateLinks> statesToLinks;
		map<NotePos, shared_ptr<NoteRowPossibleStates>>& parent;

		NotePos previousPosition;
		NotePos position;
		NoteRow<rowSize> noteRow;
		bool allowDoublesteps;

	public:
		NoteRowPossibleStates(NotePos position, NotePos previousPosition, map<NotePos, shared_ptr<NoteRowPossibleStates>>& parent,
			const Chart<rowSize>& chart, bool allowDoublesteps);

		void InsertPossibleStates()
		{
			if (previousPosition == -1)
			{
				auto emptyState = make_shared<TState>(States<rowSize>::GetEmpty());
				InsertPossibleStatesFor(emptyState, nullopt);
			}
			else
			{
				shared_ptr<NoteRowPossibleStates> previousStates = parent.at(previousPosition);
				for (const auto& previousState : previousStates->GetStates())
				{
					auto previousStateLinks = previousStates->GetStateLinks(*previousState);
					InsertPossibleStatesFor(previousState, previousStateLinks);
				}
			}
		}

		void InsertPossibleStatesFor(const shared_ptr<TState>& previousState, TOptionalStateLinks previousStateLinks)
		{
			auto [tappables, tappablesCount] = noteRow.GetTappablesWithCount();

			if (tappablesCount == 0)
			{
				TapAndInsertState(previousState, previousState->Before(noteRow), previousStateLinks, TLimbsOnPads::Empty);
			}
			else if (tappablesCount == 1)
			{
				InsertSingleTapStates(previousState, previousStateLinks);
			}
			else if (tappablesCount > 1)
			{
				InsertMultipleTapsStates(previousState, previousStateLinks);
			}
		}

		void InsertSingleTapStates(const shared_ptr<TState> previousState, TOptionalStateLinks previousStateLinks)
		{
			TState beforeTapState = previousState->Before(noteRow);

			auto panel = noteRow.GetFirstTappable();

			// If there already is a limb on that panel, use it.
			auto occupyingLimb = beforeTapState.GetOccupiedPanels()[panel];
			if (occupyingLimb != Limb::None)
			{
				TapAndInsertState(previousState, beforeTapState, previousStateLinks, panel, occupyingLimb);
				return;
			}

			// Disallow doublesteps -- force alternate stepping
			if (!allowDoublesteps)
			{
				Limb nextLeg = beforeTapState.GetNextFreeLeg();

				// If there's no next free leg, try proposing both legs if they're free
				if (nextLeg == Limb::None)
				{
					bool done = false;
					if (beforeTapState.IsFree(Limb::LeftLeg))
					{
						TapAndInsertState(previousState, beforeTapState, previousStateLinks, panel, Limb::LeftLeg);
						done = true;
					}

					if (beforeTapState.IsFree(Limb::RightLeg))
					{
						TapAndInsertState(previousState, beforeTapState, previousStateLinks, panel, Limb::RightLeg);
						done = true;
					}

					if (done) return;
					// continue if neither legs were free
				}

				// If there is a next free leg, use only that
				else
				{
					TapAndInsertState(previousState, beforeTapState, previousStateLinks, panel, nextLeg);
					return;
				}
			}
			// Allow doublesteps
			else
			{
				if (beforeTapState.IsFree(Limb::LeftLeg))
					TapAndInsertState(previousState, beforeTapState, previousStateLinks, panel, Limb::LeftLeg);
				if (beforeTapState.IsFree(Limb::RightLeg))
					TapAndInsertState(previousState, beforeTapState, previousStateLinks, panel, Limb::RightLeg);
				if (beforeTapState.IsFree(Limb::LeftLeg) || beforeTapState.IsFree(Limb::RightLeg))
					return;
			}

			// Pick a hand depending on foot placement
			// If the preferred hand is reserved, use the another one
			Panel leftLegPanel = beforeTapState.GetOccupyingPanel(Limb::LeftLeg);
			Panel rightLegPanel = beforeTapState.GetOccupyingPanel(Limb::RightLeg);
			FeetPlacement placement = Computations::GetFeetPlacement(leftLegPanel, rightLegPanel);
			Limb hand;
			if (placement.GetLeftHandPanel() == panel) hand = Limb::LeftHand;
			else hand = Limb::RightHand;
			if (!beforeTapState.IsFree(hand)) hand = Limbs::GetPair(hand);
			TapAndInsertState(previousState, beforeTapState, previousStateLinks, panel, hand);
		}

		void InsertMultipleTapsStates(const shared_ptr<TState> previousState, TOptionalStateLinks previousStateLinks)
		{
			TState beforeTapState = previousState->Before(noteRow);
			auto freeLimbs = beforeTapState.GetFreeLimbs();
			int freeLegsAmount = beforeTapState.GetNumberOfFreeLegs();

			/*
			 * Create different placements for legs we have and then augment the
			 * placements with hands according to the FeetPlacement class.
			 */

			// There's at least one free leg...
			if (freeLegsAmount >= 1)
			{
				// ... so put it everywhere it can go
				noteRow.ForEachTappable([&](Panel tapPanel1)
				{
					// There's another free leg
					if (freeLegsAmount == 2)
					{
						// ... so put it in the remaining places
						noteRow.ForEachTappable([&](Panel tapPanel2)
						{
							if (tapPanel1 != tapPanel2)
							{
								// Do this for every formed limbsUsed (2 legged)
								FillHandsAndInsert(tapPanel1, tapPanel2, freeLimbs, 2, previousState, beforeTapState,
									previousStateLinks);
							}
						});
					}
					// There was only one free leg
					else
					{
						// Do this for every formed limbsUsed (1 legged)
						FillHandsAndInsert(tapPanel1, Panel::None, freeLimbs, 1, previousState, beforeTapState,
							previousStateLinks);
					}
				});
			}
			// Seems that all legs are reserved
			else
			{
				// Do this for empty limbsUsed
				FillHandsAndInsert(Panel::None, Panel::None, freeLimbs, 0, previousState, beforeTapState,
					previousStateLinks);
			}
		}

		void FillHandsAndInsert(Panel tapPanel1, Panel tapPanel2, Limb/*bitmask*/ freeLimbs,
			int handsOffset, const shared_ptr<TState> previousState, const TState& beforeTapState, TOptionalStateLinks previousStateLinks)
		{
			TLimbsOnPad limbsUsed;

			auto leftLegPanel = Panel::None, rightLegPanel = Panel::None;

			// Find out where the legs are
			if (handsOffset == 2)
			{
				// Two legs were placed
				limbsUsed[tapPanel1] = Limb::LeftLeg;
				limbsUsed[tapPanel2] = Limb::RightLeg;
				leftLegPanel = tapPanel1;
				rightLegPanel = tapPanel2;
			}
			else if (handsOffset == 1)
			{
				// One leg was placed
				auto leg = Limbs::FilterLegs(freeLimbs);
				limbsUsed[tapPanel1] = leg;

				if (leg == Limb::LeftLeg)
				{
					leftLegPanel = tapPanel1;
					rightLegPanel = beforeTapState.GetOccupyingPanel(Limb::RightLeg);
				}
				else
				{
					leftLegPanel = beforeTapState.GetOccupyingPanel(Limb::LeftLeg);
					rightLegPanel = tapPanel1;
				}
			}
			else
			{
				// No legs were placed
				leftLegPanel = beforeTapState.GetOccupyingPanel(Limb::LeftLeg);
				rightLegPanel = beforeTapState.GetOccupyingPanel(Limb::RightLeg);
			}

			// At this point, both legs are placed or reserved. Now let's place hands.
			// This FeetPlacement will tell how the hands should be placed...
			FeetPlacement placement = Computations::GetFeetPlacement(leftLegPanel, rightLegPanel);
			Panel leftHandPanel = placement.GetLeftHandPanel();
			Panel rightHandPanel = placement.GetRightHandPanel();

			if (Limbs::Contains(freeLimbs, Limb::LeftHand))
			{
				if (NoteTypes::IsTappable(noteRow[leftHandPanel]) && beforeTapState.IsFree(Limb::LeftHand))
				{
					if (limbsUsed[leftHandPanel] == Limb::None) limbsUsed[leftHandPanel] = Limb::LeftHand;
				}
			}
			if (Limbs::Contains(freeLimbs, Limb::RightHand))
			{
				if (NoteTypes::IsTappable(noteRow[rightHandPanel]) && beforeTapState.IsFree(Limb::RightHand))
				{
					if (limbsUsed[rightHandPanel] == Limb::None) limbsUsed[rightHandPanel] = Limb::RightHand;
				}
			}

			// We might have panels that couldn't be pressed with FeetPlacement hand orientation
			// So we unfortunately have to ensure that we did fill all tappable panels
			// And fill them in the opposite way if they're unfilled
			if (NoteTypes::IsTappable(noteRow[leftHandPanel]) && limbsUsed[leftHandPanel] == Limb::None)
			{
				limbsUsed[leftHandPanel] = Limb::RightHand;
			}

			if (NoteTypes::IsTappable(noteRow[rightHandPanel]) && limbsUsed[rightHandPanel] == Limb::None)
			{
				limbsUsed[rightHandPanel] = Limb::LeftHand;
			}

			// Now limbsUsed is filled with hands too, let's insert the state
			TapAndInsertState(previousState, beforeTapState, previousStateLinks, limbsUsed);
		}

		void TapAndInsertState(const shared_ptr<TState> previousState, const TState& beforeTapState, TOptionalStateLinks previousStateLinks,
			Panel panel, Limb limb)
		{
			TLimbsOnPad limbsUsed;
			limbsUsed[panel] = limb;
			TapAndInsertState(previousState, beforeTapState, previousStateLinks, limbsUsed);
		}

		void TapAndInsertState(const shared_ptr<TState> previousState, const TState& beforeTapState, TOptionalStateLinks previousStateLinks, const TLimbsOnPad& limbsUsed)
		{
			TState state = beforeTapState.Tap(noteRow, limbsUsed);

			auto stateLinksItr = statesToLinks.find(state);
			if (stateLinksItr == statesToLinks.end())
			{
				statesToLinks[state] = TStateLinks{};
				auto statePtr = make_shared<TState>(state);
				states.push_back(statePtr);

				if (previousStateLinks)
				{
					LinkStates(previousState, previousStateLinks.value(), statePtr, statesToLinks[state]);
				}
			}
			else
			{
				if (previousStateLinks)
				{
					// Note: this state is now not in any NoteRowPossibleStates::states, not sure if we should be using pointers to states really
					auto statePtr = make_shared<TState>(state); 
					LinkStates(previousState, previousStateLinks.value(), statePtr, stateLinksItr->second);
				}
			}
		}

		static void LinkStates(const shared_ptr<TState> parent, TStateLinks& parentLinks, const shared_ptr<TState> child, TStateLinks& childLinks)
		{
			parentLinks.linksFrom.insert(child);
			childLinks.linksTo.insert(parent);
		}

		const vector<shared_ptr<TState>>& GetStates() const
		{
			return states;
		}

		const TStateLinks& GetStateLinks(const TState& state) const
		{
			return statesToLinks.at(state);
		}

		const TState& GetCheapestState()
		{
			int lowestCost = INT_MAX;
			auto lowest = statesToLinks.end();
			for (auto it = statesToLinks.begin(); it != statesToLinks.end(); it++)
			{
				int cost = it->second.costToGoal;
				if (cost < lowestCost)
				{
					lowestCost = cost;
					lowest = it;
				}
			}
			
			if (lowest != statesToLinks.end())
				return lowest->first;

			throw range_error("statesToLinks is empty");
		}
	};

	template<size_t rowSize>
	NoteRowPossibleStates<rowSize>::NoteRowPossibleStates(NotePos position, NotePos previousPosition, map<NotePos, shared_ptr<NoteRowPossibleStates>>& parent,
			const Chart<rowSize>& chart, bool allowDoublesteps) :
		states{},
		statesToLinks{},
		parent{parent},
		previousPosition{previousPosition},
		position{position},
		allowDoublesteps{allowDoublesteps}
	{
		noteRow = chart.GetNoteRow(position);
	}
}