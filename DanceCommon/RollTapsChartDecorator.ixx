export module RollTapsChartDecorator;

import StdCore;
import Song;
import Chart;
import NotePos;
import NoteRow;
import NoteData;
import Panel;
import NoteLength;

namespace DanceCommon
{
	export template<size_t rowSize>
	class RollTapsChartDecorator
	{
		static constexpr array RollTapDistances
		{
			NoteLength::Note_4th,
			NoteLength::Note_8th,
			NoteLength::Note_16th,
			NoteLength::Note_32nd
		};

		static inline const double RollReleaseDuration = 0.32;

		using TChart = Chart<rowSize>;
		using TNoteRow = NoteRow<rowSize>;

		const Song& song;
		const TChart& c;
		NoteData<rowSize> rollTaps;

	public:
		RollTapsChartDecorator(const Song& song, const TChart& chart) :
			song{song},
			c{chart}
		{}

		void InsertAllRollTaps()
		{
			for (NotePos position = 0; c.Contains(position); position = c.NextPosition(position)) 
			{
				NoteRow noteRow = c.GetNoteRow(position);
				if (noteRow.GetHoldables() != Panel::None) 
				{
					noteRow.ForEachRollStart([&](Panel panel)
					{
						NotePos rollEndPosition = c.FindHoldEnd(position, false, (int)panel);

						if (rollEndPosition == -1) 
							throw invalid_argument("Could not find roll end");

						InsertRollTaps(position, rollEndPosition, panel);
					});
				}
			}
		}

		void InsertRollTaps(NotePos rollStartPosition, NotePos rollEndPosition, Panel panel)
		{
			int distance = CalculateRollTapDistance(rollStartPosition);

			for (NotePos pos = rollStartPosition; pos <= rollEndPosition; pos += distance)
			{
				rollTaps.SetNote(pos, NoteType.VirtualTap, (int)panel, false);
			}

			if (rollTaps.GetNote(rollEndPosition, (int)panel) == NoteType.Empty)
			{
				rollTaps.SetNote(rollEndPosition, NoteType.HoldEnd, (int)panel, false);
			}
		}

		int CalculateRollTapDistance(NotePos rollStartPosition)
		{
			double bpm = song.GetMetadata().GetBpm(rollStartPosition);
			double secondsPerBeat = 60.0 / bpm;

			for (NoteLength distance : RollTapDistances)
			{
				if (secondsPerBeat * NotePositions::ToBeatsDouble((NotePos)distance) <= RollReleaseDuration)
				{
					return (int)distance;
				}
			}

			return INT_MAX;
		}

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