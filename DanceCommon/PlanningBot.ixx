export module PlanningBot;

import StdCore;
import Chart;
import RobotPlay;
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
		using TChart = Chart<rowSize>;
		using TRobotPlay = RobotPlay<rowSize>;
		using TNoteRowPossibleStates = NoteRowPossibleStates<rowSize>;
		using TStateLinks = StateLinks<rowSize>;
		using TStateLinksUtils = StateLinksUtils<rowSize>;
		using TState = State<rowSize>;

		bool allowDoublesteps = false;

	public:
		TRobotPlay Play(const TChart& chart)
		{
			// We want roll taps in the chart
			//if (!(chart is RollTapsChartDecorator))
			//	chart = new RollTapsChartDecorator(chart);
			// TODO roll taps

			auto rowPossibleStates = GenerateStateGraph(chart);
			CalculateCostsToGoal(rowPossibleStates);

			return TRobotPlay(rowPossibleStates);
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
				auto rowStates = make_shared<TNoteRowPossibleStates>(position, previousPosition, rowPossibleStates, chart, allowDoublesteps);
				rowStates->InsertPossibleStates();
				rowPossibleStates[position] = rowStates;

				previousPosition = position;
			}

			return rowPossibleStates;
		}

		void CalculateCostsToGoal(const map<NotePos, shared_ptr<TNoteRowPossibleStates>>& rowPossibleStates)
		{
			if (rowPossibleStates.size() == 0) return;

			shared_ptr<TNoteRowPossibleStates> previousStates = nullptr;

			for (auto it = rowPossibleStates.rbegin(); it != rowPossibleStates.rend(); it++)
			{
				shared_ptr<TNoteRowPossibleStates> states = it->second;
				for (auto state : states->GetStates())
				{
					auto stateLinks = states->GetStateLinks(*state);
					int lowestCost = previousStates != nullptr
						? TStateLinksUtils::GetCheapestChildCost(stateLinks, *previousStates)
						: 0;
					stateLinks.costToGoal = state->GetCost() + lowestCost;
				}

				previousStates = states;
			}
		}
	};

	export using SinglesPlanningBot = PlanningBot<NoteRowSize::Single>;
}