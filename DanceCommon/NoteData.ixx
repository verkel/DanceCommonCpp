export module NoteData;
import PlayStyle;
import NotePos;
import NoteRow;
import <vector>;

export namespace DanceCommon
{
	export template<size_t rowSize>
	class NoteData
	{
	private:
		using TNoteRow = NoteRow<rowSize>;

		std::vector<TNoteRow> noteRows;

	public:
		inline static const TNoteRow EmptyRow{};

		NoteData() :
			noteRows(0)
		{
		}

		const TNoteRow& GetNoteRow(NotePos notePosition)
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

		inline bool Contains(NotePos position)
		{
			return position >= 0 && position < noteRows.size();
		}

		inline NotePos NextPosition(NotePos position)
		{
			int pos = position + 1;
			NotePos upperBound = (NotePos)noteRows.size();
			while (pos < upperBound && noteRows[pos].IsEmpty())
			{
				pos++;
			}
			return pos;
		}

		NotePos PreviousPosition(NotePos position)
		{
			return PreviousPosition(position, -1);
		}

		NotePos PreviousPosition(NotePos position, int skipEmptyPanel)
		{
			return PreviousPosition(position, skipEmptyPanel, NotePositions::Invalid);
		}

		NotePos PreviousPosition(NotePos position, int skipEmptyPanel, NotePos lowerBound)
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
				while (GetNoteRow(pos).GetNote(skipEmptyPanel) == NoteType::Empty && pos > lowerBound)
				{
					pos--;
				}
			}

			return pos;
		}

		NotePos GetLastPosition()
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