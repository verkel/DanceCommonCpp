export module NoteType;
import <array>;
import <format>;
import ParseException;

export namespace DanceCommon
{
	export enum class NoteType : unsigned char
	{
		Empty,
		Tap,
		HoldStart,
		HoldEnd,
		RollStart,
		Mine,
		VirtualTap
	};

	export class NoteTypes
	{
	public:
		static NoteType Get(char c)
		{
			switch (c)
			{
			case '0':
				return NoteType::Empty;
			case '1':
				return NoteType::Tap;
			case '2':
				return NoteType::HoldStart;
			case '3':
				return NoteType::HoldEnd;
			case '4':
				return NoteType::RollStart;
			case 'M':
				return NoteType::Mine;
			case 'V':
				return NoteType::VirtualTap;
			}

			throw ParseException(std::format("Unknown note type: {}", c));
		}
	};
}