export module NoteType;

import StdCore;
import ParseException;

export namespace DanceCommon
{
	export enum class NoteType : unsigned char
	{
		Empty = 0,
		Tap = 1 << 1,
		HoldStart = 1 << 2,
		HoldEnd = 1 << 3,
		RollStart = 1 << 4,
		Mine = 1 << 5,
		VirtualTap = 1 << 6
	};

	inline NoteType operator|(NoteType a, NoteType b)
	{
	    return static_cast<NoteType>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline NoteType operator&(NoteType a, NoteType b)
	{
	    return static_cast<NoteType>(static_cast<int>(a) & static_cast<int>(b));
	}

	inline NoteType operator~(NoteType a)
	{
	    return static_cast<NoteType>(~static_cast<int>(a));
	}

	export class NoteTypes
	{
	public:
		static inline const NoteType Holdable = NoteType::HoldStart | NoteType::RollStart;
		static inline const NoteType Tap = NoteType::Tap | NoteType::VirtualTap;
		static inline const NoteType Tappable = Tap | Holdable;

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

		static bool IsTappable(NoteType nt) {
			return IsTap(nt) || IsHoldable(nt);
		}

		static bool IsHoldable(NoteType nt) {
			return nt == NoteType::HoldStart || nt == NoteType::RollStart;
		}

		static bool IsTap(NoteType nt)
		{
			return nt == NoteType::Tap || nt == NoteType::VirtualTap;
		}

		static bool Is(NoteType type, NoteType typeMask)
		{
			return (typeMask & type) != NoteType::Empty;
		}
	};
}