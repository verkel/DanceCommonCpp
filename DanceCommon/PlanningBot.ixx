export module PlanningBot;

import Chart;
import RobotPlay;
import PlayStyle;

namespace DanceCommon
{
	export template<size_t rowSize>
	class PlanningBot
	{
		using TChart = Chart<rowSize>;
		using TRobotPlay = RobotPlay<rowSize>;

		bool allowDoublesteps;

	public:
		TRobotPlay Play(const TChart& chart)
		{
			return TRobotPlay{};
		}

		void SetAllowDoublesteps(bool allowDoublesteps)
		{
			this->allowDoublesteps = allowDoublesteps;
		}
	};

	export using SinglesPlanningBot = PlanningBot<NoteRowSize::Single>;
}