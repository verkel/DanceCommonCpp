export module NoteLength;
import <array>;

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
	};
}