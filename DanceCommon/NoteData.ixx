export module NoteData;
import PlayStyle;
import NotePos;
import NoteRow;
import <vector>;

export namespace DanceCommon
{
	export template<NoteRowSize rowSize>
	class NoteData
	{
	private:
		typedef NoteRow<rowSize> TNoteRow;

		std::vector<TNoteRow> noteRows;

	public:
		NoteData() :
			noteRows(0)
		{
		}

		TNoteRow GetNoteRow(NotePos notePosition)
		{
			if (!Contains(notePosition)) 
				return TNoteRow{ };

			return noteRows[notePosition];
		}

		void SetNoteRow(NotePos notePosition, TNoteRow newRow, bool cleanupHolds)
		{
			TNoteRow oldRow = GetNoteRow(notePosition);
			EnsureNoteRowsCapacity(notePosition);
			noteRows[notePosition] = newRow;

			if (cleanupHolds) CleanupHolds(notePosition, oldRow, newRow);
		}

		bool Contains(NotePos position)
		{
			return position >= 0 && position < noteRows.size();
		}

		void EnsureNoteRowsCapacity(NotePos notePosition)
		{
			/*while (noteRows.Count <= notePosition)
			{
				noteRows.Add(null);
			}*/
		}

		void CleanupHolds(NotePos position, TNoteRow oldRow, TNoteRow newRow)
		{

		}
	};
}