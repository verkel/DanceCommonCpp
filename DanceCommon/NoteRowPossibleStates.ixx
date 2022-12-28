export module NoteRowPossibleStates;
import <vector>;
import <map>;
import <unordered_map>;
import <memory>;
import <optional>;
import State;
import StateLinks;
import NotePos;
import NoteRow;
import Chart;
import Computations;
import FeetPlacement;

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
		using TOptionalStateLinks = std::optional<std::reference_wrapper<TStateLinks>>;

	private:
		std::vector<std::shared_ptr<TState>> states;
		std::unordered_map<TState, TStateLinks> statesToLinks;
		std::shared_ptr<std::map<NotePos, NoteRowPossibleStates>> parent;

		NotePos previousPosition;
		NotePos position;
		NoteRow<rowSize> noteRow;
		bool allowDoublesteps;

	public:
		NoteRowPossibleStates(NotePos position, NotePos previousPosition, std::shared_ptr<std::map<NotePos, NoteRowPossibleStates>> parent,
			const Chart<rowSize>& chart, bool allowDoublesteps);

		void InsertPossibleStates()
		{
			if (previousPosition == -1)
			{
				auto emptyState = std::make_shared<TState>(States<rowSize>::GetEmpty());
				InsertPossibleStatesFor(emptyState, std::nullopt);
			}
			else
			{
				NoteRowPossibleStates& previousStates = parent->at(previousPosition);
				for (const auto& previousState : previousStates.GetStates())
				{
					auto previousStateLinks = previousStates.GetStateLinks(*previousState);
					InsertPossibleStatesFor(previousState, previousStateLinks);
				}
			}
		}

		void InsertPossibleStatesFor(const std::shared_ptr<TState>& previousState, TOptionalStateLinks previousStateLinks)
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

		void InsertSingleTapStates(const std::shared_ptr<TState> previousState, TOptionalStateLinks previousStateLinks)
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
			FeetPlacement placement = Computations::GetInstance().feetPlacements.Get(leftLegPanel, rightLegPanel);
			Limb hand;
			if (placement.GetLeftHandPanel() == panel) hand = Limb::LeftHand;
			else hand = Limb::RightHand;
			if (!beforeTapState.IsFree(hand)) hand = Limbs::GetPair(hand);
			TapAndInsertState(previousState, beforeTapState, previousStateLinks, panel, hand);
		}

		void InsertMultipleTapsStates(const std::shared_ptr<TState> previousState, TOptionalStateLinks previousStateLinks)
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
			int handsOffset, const std::shared_ptr<TState> previousState, const TState& beforeTapState, TOptionalStateLinks previousStateLinks)
		{
			// TODO implement
		}

		void TapAndInsertState(const std::shared_ptr<TState> previousState, const TState& beforeTapState, TOptionalStateLinks previousStateLinks,
			Panel panel, Limb limb)
		{
			TLimbsOnPad limbsUsed;
			limbsUsed[panel] = limb;
			TapAndInsertState(previousState, beforeTapState, previousStateLinks, limbsUsed);
		}

		void TapAndInsertState(const std::shared_ptr<TState> previousState, const TState& beforeTapState, TOptionalStateLinks previousStateLinks, const TLimbsOnPad& limbsUsed)
		{
			TState state = beforeTapState.Tap(noteRow, limbsUsed);

			auto stateLinksItr = statesToLinks.find(state);
			if (stateLinksItr == statesToLinks.end())
			{
				statesToLinks[state] = TStateLinks{};
				auto statePtr = std::make_shared<TState>(state);
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
					auto statePtr = std::make_shared<TState>(state); 
					LinkStates(previousState, previousStateLinks.value(), statePtr, stateLinksItr->second);
				}
			}
		}

		static void LinkStates(const std::shared_ptr<TState> parent, TStateLinks& parentLinks, const std::shared_ptr<TState> child, TStateLinks& childLinks)
		{
			parentLinks.linksFrom.insert(child);
			childLinks.linksTo.insert(parent);
		}

		const auto GetStates() const
		{
			return states;
		}

		const TStateLinks& GetStateLinks(const TState& state)
		{
			return statesToLinks.at(state);
		}
	};

	template<size_t rowSize>
	NoteRowPossibleStates<rowSize>::NoteRowPossibleStates(NotePos position, NotePos previousPosition, std::shared_ptr<std::map<NotePos, NoteRowPossibleStates>> parent,
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