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
		}

		void InsertMultipleTapsStates(const std::shared_ptr<TState> previousState, TOptionalStateLinks previousStateLinks)
		{
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