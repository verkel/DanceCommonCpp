export module RollTapsChartDecorator;

import StdCore;
import Chart;
import NotePos;
import NoteRow;

namespace DanceCommon
{
	export template<size_t rowSize>
	class RollTapsChartDecorator
	{
		using TChart = Chart<rowSize>;
		using TNoteRow = NoteRow<rowSize>;

		const TChart& c;

	public:
		RollTapsChartDecorator(const TChart& chart) :
			c{chart}
		{ }

		TNoteRow GetNoteRow(NotePos pos) const
		{
			return c.GetNoteRow(pos);
		}

		bool Contains(NotePos pos) const
		{
			return c.Contains(pos);
		}

		NotePos NextPosition(NotePos pos) const
		{
			return c.NextPosition(pos);
		}
	};
}