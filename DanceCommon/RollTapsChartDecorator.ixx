export module RollTapsChartDecorator;

import StdCore;
import Song;
import Chart;
import NotePos;
import NoteType;
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
		NoteType rollTapType;
		NoteData<rowSize> rollTaps;

	public:
		RollTapsChartDecorator(const Song& song, const TChart& chart, bool virtualTaps) :
			song{song},
			c{chart},
			rollTapType{virtualTaps ? NoteType::VirtualTap : NoteType::Tap}
		{
			InsertAllRollTaps();
		}

		void InsertAllRollTaps()
		{
			for (NotePos position = 0; c.Contains(position); position = c.NextPosition(position)) 
			{
				NoteRow noteRow = c.GetNoteRow(position);
				if (noteRow.GetHoldables() != Panel::None) 
				{
					noteRow.ForEachRollStart([&](Panel panel)
					{
						NotePos rollEndPosition = c.FindHoldEnd(position, false, Panels::Index(panel));

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
				rollTaps.SetNote(pos, rollTapType, Panels::Index(panel), false);
			}

			if (rollTaps.GetNote(rollEndPosition, Panels::Index(panel)) == NoteType::Empty)
			{
				rollTaps.SetNote(rollEndPosition, NoteType::HoldEnd, Panels::Index(panel), false);
			}
		}

		int CalculateRollTapDistance(NotePos rollStartPosition)
		{
			double bpm = song.GetMetadata().GetBpm(rollStartPosition);
			double secondsPerBeat = 60.0 / bpm;

			for (NoteLength distance : RollTapDistances)
			{
				if (secondsPerBeat * NotePositions::ToBeatsDouble(static_cast<NotePos>(distance)) <= RollReleaseDuration)
				{
					return static_cast<int>(distance);
				}
			}

			return INT_MAX;
		}

		TNoteRow GetNoteRow(NotePos pos) const
		{
			TNoteRow rollTapsRow = rollTaps.GetNoteRow(pos);
			TNoteRow noteRow = c.GetNoteRow(pos);
			return MergeRows(noteRow, rollTapsRow);
		}

		TNoteRow MergeRows(TNoteRow noteRow, TNoteRow rollTapsRow) const
		{
			if (rollTapsRow.IsEmpty()) 
				return noteRow;

			if (noteRow.IsEmpty()) 
				return rollTapsRow;

			TNoteRow mergedRow;
			for (int i = 0; i < rowSize; i++)
			{
				if (rollTapsRow[i] == rollTapType)
				{
					mergedRow[i] = rollTapType;
				}
				else if (noteRow[i] == NoteType::HoldEnd && rollTapsRow[i] == NoteType::HoldEnd)
				{
					mergedRow[i] = NoteType::Empty;
				}
				else
				{
					mergedRow[i] = noteRow[i];
				}
			}

			return mergedRow;
		}

		bool Contains(NotePos pos) const
		{
			return c.Contains(pos);
		}

		NotePos NextPosition(NotePos position) const
		{
			return NextPosition(position, -1);
		}

		NotePos NextPosition(NotePos position, int skipEmptyPanel) const
		{
			NotePos nextPos = c.NextPosition(position, skipEmptyPanel);
			NotePos rollTapsNextPos = rollTaps.NextPosition(position, skipEmptyPanel, nextPos);
			return std::min(static_cast<int>(rollTapsNextPos), static_cast<int>(nextPos));
		}
	};
}