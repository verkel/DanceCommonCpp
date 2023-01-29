export module NoteData;

import StdCore;
import PlayStyle;
import NotePos;
import NoteRow;
import NoteType;

export namespace DanceCommon
{
	export template<size_t rowSize>
	class NoteData
	{
	private:
		using TNoteRow = NoteRow<rowSize>;

		vector<TNoteRow> noteRows;

	public:
		inline static const TNoteRow EmptyRow{};

		NoteData() :
			noteRows(0)
		{
		}

		const TNoteRow& GetNoteRow(NotePos notePosition) const
		{
			if (!Contains(notePosition))
				return EmptyRow;

			return noteRows[notePosition];
		}

		void SetNoteRow(NotePos notePosition, TNoteRow newRow, bool cleanupHolds)
		{
			TNoteRow oldRow = GetNoteRow(notePosition);
			EnsureNoteRowsCapacity(notePosition);
			noteRows[notePosition] = newRow;

			if (cleanupHolds) CleanupHolds(notePosition, oldRow, newRow);
		}

		NoteType GetNote(NotePos notePosition, int panel)
		{
			return GetNoteRow(notePosition)[panel];
		}

		bool Contains(NotePos position) const
		{
			return position >= 0 && position < noteRows.size();
		}

		NotePos NextPosition(NotePos position) const
		{
			int pos = position + 1;
			NotePos upperBound = (NotePos)noteRows.size();
			while (pos < upperBound && noteRows[pos].IsEmpty())
			{
				pos++;
			}
			return pos;
		}

		NotePos PreviousPosition(NotePos position) const
		{
			return PreviousPosition(position, -1);
		}

		NotePos PreviousPosition(NotePos position, int skipEmptyPanel) const
		{
			return PreviousPosition(position, skipEmptyPanel, NotePositions::Invalid);
		}

		NotePos PreviousPosition(NotePos position, int skipEmptyPanel, NotePos lowerBound) const
		{
			NotePos pos = position - 1;

			if (skipEmptyPanel == -1)
			{
				while (GetNoteRow(pos).IsEmpty() && pos > lowerBound)
				{
					pos--;
				}
			}
			else
			{
				while (GetNoteRow(pos)[skipEmptyPanel] == NoteType::Empty && pos > lowerBound)
				{
					pos--;
				}
			}

			return pos;
		}

		NotePos GetLastPosition() const
		{
			if (noteRows.size() == 0)
				return NotePositions::Invalid;

			NotePos pos = (NotePos)(noteRows.size() - 1);
			if (GetNoteRow(pos).IsEmpty())
			{
				return PreviousPosition(pos);
			}
			else
			{
				return pos;
			}
		}
		
		NotePos FindHoldEnd(NotePos notePosition, bool includeThisPosition, int panel)
		{
			if (!Contains(notePosition)) 
				return NotePositions::Invalid;

			if (includeThisPosition && GetNote(notePosition, panel) == NoteType::HoldEnd)
				return notePosition;

			NotePos nextPosition = NextPosition(notePosition, panel);

			if (!Contains(nextPosition)) 
				return NotePositions::Invalid;

			NoteType note = GetNote(nextPosition, panel);

			if (note == NoteType::HoldEnd) 
				return nextPosition;
			else 
				return NotePositions::Invalid;
		}

		void EnsureNoteRowsCapacity(NotePos notePosition)
		{
			size_t expectedSize = (size_t)notePosition + 1;
			if (noteRows.size() < expectedSize)
			{
				noteRows.resize(expectedSize);
			}
		}

		void CleanupHolds(NotePos position, TNoteRow oldRow, TNoteRow newRow)
		{

		}

		NotePos GetLength() const
		{
			return (NotePos)noteRows.size();
		}
	};
}