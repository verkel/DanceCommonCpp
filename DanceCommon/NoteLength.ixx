export module NoteLength;
import <array>;
import <stdexcept>;

export namespace DanceCommon
{
	export enum class NoteLength
	{
		Measure = 192,
		Note_4th = 48,
		Note_8th = 24,
		Note_12th = 16,
		Note_16th = 12,
		Note_24th = 8,
		Note_32nd = 6,
		Note_48th = 4,
		Note_64th = 3,
		Note_96th = 2,
		Note_192nd = 1,
		None = 0
	};

	export class NoteLengths
	{
	public:

		/**
		 * All the NoteLengths, except Measure, Note_96th which is not usable but
		 * exists because that resolution is used when a measure contains both 12th
		 * and 32nd notes and None.
		 **/
		static constexpr std::array UsableValues
		{
			NoteLength::Note_4th,
			NoteLength::Note_8th,
			NoteLength::Note_12th,
			NoteLength::Note_16th,
			NoteLength::Note_24th,
			NoteLength::Note_32nd,
			NoteLength::Note_48th,
			NoteLength::Note_64th,
			NoteLength::Note_96th,
			NoteLength::Note_192nd
		};

		/**
		 * All the NoteLengths, except None
		 */
		static constexpr std::array Values
		{
			NoteLength::Measure,
			NoteLength::Note_4th,
			NoteLength::Note_8th,
			NoteLength::Note_12th,
			NoteLength::Note_16th,
			NoteLength::Note_24th,
			NoteLength::Note_32nd,
			NoteLength::Note_48th,
			NoteLength::Note_64th,
			NoteLength::Note_96th,
			NoteLength::Note_192nd
		};


		static int GetResolution(NoteLength length)
		{
			switch (length)
			{
				case NoteLength::Measure: return 1;
				case NoteLength::Note_4th: return 4;
				case NoteLength::Note_8th: return 8;
				case NoteLength::Note_12th: return 12;
				case NoteLength::Note_16th: return 16;
				case NoteLength::Note_24th: return 24;
				case NoteLength::Note_32nd: return 32;
				case NoteLength::Note_48th: return 48;
				case NoteLength::Note_64th: return 64;
				case NoteLength::Note_96th: return 96;
				case NoteLength::Note_192nd: return 192;
			}

			throw std::invalid_argument("length");
		}

		static NoteLength FromResolution(int resolution)
		{
			switch (resolution)
			{
				case 1: return NoteLength::Measure;
				case 4: return NoteLength::Note_4th;
				case 8: return NoteLength::Note_4th;
				case 12: return NoteLength::Note_12th;
				case 16: return NoteLength::Note_16th;
				case 24: return NoteLength::Note_24th;
				case 32: return NoteLength::Note_32nd;
				case 48: return NoteLength::Note_48th;
				case 64: return NoteLength::Note_64th;
				case 96: return NoteLength::Note_96th;
				case 192: return NoteLength::Note_192nd;
			}

			throw std::invalid_argument("resolution");
		}
	};
}