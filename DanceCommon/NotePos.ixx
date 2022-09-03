export module NotePos;
import NoteLength;
import <array>;

export namespace DanceCommon
{
	export typedef int NotePos;

	NotePos operator * (NoteLength length, int n)
	{
		return (int)length * n;
	}

	int operator / (NotePos notePos, NoteLength noteLength)
	{
		return notePos / (int)noteLength;
	}

	NotePos operator + (NotePos notePos, NoteLength noteLength)
	{
		return notePos + (int)noteLength;
	}

	export class NotePositions 
	{
	private:
		static constexpr auto GetMeasureNoteLengths()
		{
			std::array<NoteLength, (int)NoteLength::Measure> measureNoteLengths;

			for (auto length : NoteLengths::Values)
			{
				NotePos pos = 0;
				for (int n = 0; (pos = length * n) < (int)NoteLength::Measure; n++)
				{
					if (measureNoteLengths[pos] == NoteLength::None)
					{
						measureNoteLengths[pos] = length;
					}
				}
			}

			return measureNoteLengths;
		}

	public:
		const NotePos Invalid = -1;
		const NotePos First = 0;

		/**
		 * Get the NoteLength at the given note position.
		 *
		 * @return the NoteLength
		 */
		static NoteLength GetNoteLength(NotePos notePos)
		{
			int localPosition = notePos % (int)NoteLength::Measure;
			return GetMeasureNoteLengths()[localPosition];
		}

		/**
		 * Floor the note position to the given NoteLength.
		 * <p>
		 * For example, to get the note position of the measure the given position is
		 * in: <br>
		 * <code>
		 * int measurePosition = NotePosition.floor(NoteLength.Measure);
		 * </code>
		 *
		 * @param notePosition
		 * @param noteLength
		 * @return
		 */
		static NotePos Floor(NotePos notePos, NoteLength noteLength)
		{
			return noteLength * (notePos / noteLength);
		}

		/**
		 * Ceil the note position to the given NoteLength.
		 *
		 * @param notePosition
		 * @param noteLength
		 * @return
		 */
		static NotePos Ceil(NotePos notePos, NoteLength noteLength)
		{
			if (notePos % (int)noteLength == 0) 
				return notePos;
			else 
				return Floor(notePos, noteLength) + noteLength;
		}
	};

}