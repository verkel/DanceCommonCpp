export module NotePos;
import NoteLength;
import <array>;

export namespace DanceCommon
{
	export using NotePos = int;

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

	struct MeasureNoteLengths
	{
		std::array<NoteLength, (int)NoteLength::Measure> data;

		constexpr MeasureNoteLengths() : data{}
		{
			for (auto length : NoteLengths::Values)
			{
				int pos = 0;
				for (int n = 0; (pos = (int)length * n) < (int)NoteLength::Measure; n++)
				{
					if (data[pos] == NoteLength::None)
					{
						data[pos] = length;
					}
				}
			}
		}
	};

	constexpr MeasureNoteLengths measureNoteLengths;

	export class NotePositions 
	{
	private:

	public:
		static const NotePos Invalid = -1;
		static const NotePos First = 0;

		/**
		 * Get the NoteLength at the given note position.
		 *
		 * @return the NoteLength
		 */
		static NoteLength GetNoteLength(NotePos notePos)
		{
			int localPosition = notePos % (int)NoteLength::Measure;
			return measureNoteLengths.data[localPosition];
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

		/**
		 * Convert the given note position to beats. One measure is 4 beats.
		 *
		 * @return the beat as double
		 */
		static float ToBeats(NotePos notePos) {
			return notePos / 48.0f;
		}

		/**
		 * Convert the given note position to measures.
		 *
		 * @return the beat as double
		 */
		static float ToMeasures(NotePos notePos) {
			return notePos / 192.0f;
		}

		/**
		 * Convert the given note position to beats. One measure is 4 beats.
		 *
		 * @return the beat as double
		 */
		static double ToBeatsDouble(NotePos notePos) {
			return notePos / 48.0;
		}

		/**
		 * Convert the given note position to measures.
		 *
		 * @return the beat as double
		 */
		static double ToMeasuresDouble(NotePos notePos) {
			return notePos / 192.0;
		}
	};
}