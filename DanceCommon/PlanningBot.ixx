export module PlanningBot;

import StdCore;
import Chart;
import RollTapsChartDecorator;
import Play;
import PlayStyle;
import NotePos;
import NoteRowPossibleStates;
import State;
import StateLinks;
import StateLinksUtils;

namespace DanceCommon
{
	export template<size_t rowSize>
	class PlanningBot
	{
		using TChart = RollTapsChartDecorator<rowSize>;
		using TNoteRowPossibleStates = NoteRowPossibleStates<rowSize>;
		using TStateLinks = StateLinks<rowSize>;
		using TStateLinksUtils = StateLinksUtils<rowSize>;
		using TState = State<rowSize>;
		using TPlay = Play<rowSize>;

		bool allowDoublesteps = false;

	public:
		TPlay Play(const TChart& chart)
		{
			auto rowPossibleStates = GenerateStateGraph(chart);
			CalculateCostsToGoal(rowPossibleStates);
			return ComputeOptimalPlay(rowPossibleStates);
		}

		void SetAllowDoublesteps(bool allowDoublesteps)
		{
			this->allowDoublesteps = allowDoublesteps;
		}

	private:
		map<NotePos, shared_ptr<TNoteRowPossibleStates>> GenerateStateGraph(const TChart& chart)
		{
			map<NotePos, shared_ptr<TNoteRowPossibleStates>> rowPossibleStates;

			NotePos previousPosition = -1;
			for (NotePos position = 0; chart.Contains(position); position = chart.NextPosition(position))
			{
				auto rowStates = make_shared<TNoteRowPossibleStates>(position, previousPosition, rowPossibleStates, chart.GetNoteRow(position), allowDoublesteps);
				rowStates->InsertPossibleStates();
				rowPossibleStates[position] = rowStates;

				previousPosition = position;
			}

			return rowPossibleStates;
		}

		static void CalculateCostsToGoal(const map<NotePos, shared_ptr<TNoteRowPossibleStates>>& rowPossibleStates)
		{
			if (rowPossibleStates.size() == 0) return;

			shared_ptr<TNoteRowPossibleStates> previousStates = nullptr;

			for (auto it = rowPossibleStates.rbegin(); it != rowPossibleStates.rend(); it++)
			{
				shared_ptr<TNoteRowPossibleStates> states = it->second;
				for (const auto& state : states->GetStates())
				{
					auto& stateLinks = states->GetStateLinks(state);
					int lowestCost = previousStates != nullptr
						? TStateLinksUtils::GetCheapestChildCost(stateLinks, *previousStates)
						: 0;
					stateLinks.costToGoal = state.GetCost() + lowestCost;
				}

				previousStates = states;
			}
		}

		static TPlay ComputeOptimalPlay(const map<NotePos, shared_ptr<TNoteRowPossibleStates>>& rowPossibleStates)
		{
			TPlay play;

			if (rowPossibleStates.size() > 0)
			{
				auto it = rowPossibleStates.begin();

				NotePos position = it->first;
				shared_ptr<TNoteRowPossibleStates> row1PossibleStates = it->second;
				reference_wrapper<const TState> state = std::ref(row1PossibleStates->GetCheapestState());
				reference_wrapper<const TStateLinks> stateLinks = std::ref(row1PossibleStates->GetStateLinks(state));
				play[position] = state;
				SortCheapestStateFirst(*row1PossibleStates, state);
				it++;

				for (; it != rowPossibleStates.end(); it++)
				{
					position = it->first;
					shared_ptr<TNoteRowPossibleStates> states = it->second;
					state = StateLinksUtils<rowSize>::GetCheapestChild(stateLinks, *states);
					stateLinks = states->GetStateLinks(state);
					play[position] = state;
					SortCheapestStateFirst(*states, state);
				}
			}

			return play;
		}

		static void SortCheapestStateFirst(TNoteRowPossibleStates& states, const TState& cheapest)
		{
			auto statesList = states.GetStates();
			if (statesList.size() < 2) return;
			auto it = std::find(statesList.begin(), statesList.end(), cheapest);
			if (it != statesList.end())
			{
				statesList.erase(it);
			}
			else
			{
				throw exception("Failed to reorder states");
			}
			statesList.insert(statesList.begin(), cheapest);
		}
	};

	export using SinglesPlanningBot = PlanningBot<NoteRowSize::Single>;
}