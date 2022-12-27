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
		using TOptionalStateLinks = std::optional<std::reference_wrapper<TStateLinks>>;

	private:
		std::vector<TState> states;
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
				InsertPossibleStatesFor(States<rowSize>::GetEmpty(), std::nullopt);
			}
			else
			{
				NoteRowPossibleStates& previousStates = parent->at(previousPosition);
				for (auto& previousState : previousStates.GetStates())
				{
					auto previousStateLinks = previousStates.GetStateLinks(previousState);
					InsertPossibleStatesFor(previousState, previousStateLinks);
				}
			}
		}

		void InsertPossibleStatesFor(const TState& previousState, TOptionalStateLinks previousStateLinks)
		{
			auto [tappables, tappablesCount] = noteRow.GetTappablesWithCount();

			// TODO
			/*if (tappablesCount == 0)
			{
				tapAndInsertState(previousState, previousState.before(noteRow), previousStateLinks,
					new LimbsOnPad(playStyle));
			}
			else */if (tappablesCount == 1)
			{
				insertSingleTapStates(previousState, previousStateLinks);
			}
			else if (tappablesCount > 1)
			{
				insertMultipleTapsStates(previousState, previousStateLinks);
			}
		}

		void insertSingleTapStates(const TState& previousState, TOptionalStateLinks previousStateLinks)
		{
			State beforeTapState = previousState.Before(noteRow);
		}

		void insertMultipleTapsStates(const TState& previousState, TOptionalStateLinks previousStateLinks)
		{
		}

		const std::vector<TState>& GetStates() const
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