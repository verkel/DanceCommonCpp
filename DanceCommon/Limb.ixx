export module Limb;
import StdCore;

namespace DanceCommon
{
	export enum class Limb : unsigned char
	{
		None = 0,
		LeftLeg = 1 << 0,
		RightLeg = 1 << 1,
		LeftHand = 1 << 2,
		RightHand = 1 << 3,
	};

	export constexpr Limb operator|(Limb a, Limb b)
	{
		return static_cast<Limb>(static_cast<int>(a) | static_cast<int>(b));
	}

	export constexpr Limb operator&(Limb a, Limb b)
	{
		return static_cast<Limb>(static_cast<int>(a) & static_cast<int>(b));
	}

	export constexpr Limb operator~(Limb a)
	{
		return static_cast<Limb>(~static_cast<int>(a));
	}

	export class Limbs
	{
	public:
		static inline constexpr array<Limb, 4> Values =
		{
			Limb::LeftLeg,
			Limb::RightLeg,
			Limb::LeftHand,
			Limb::RightHand
		};

		static inline constexpr Limb BothLegs = Limb::LeftLeg | Limb::RightLeg;
		static inline constexpr Limb All = Limb::LeftLeg | Limb::RightLeg | Limb::LeftHand | Limb::RightHand;

		static bool IsLeg(Limb limb)
		{
			return limb == Limb::LeftLeg || limb == Limb::RightLeg
				|| limb == BothLegs;
		}

		static bool IsHand(Limb limb)
		{
			return limb == Limb::LeftHand || limb == Limb::RightHand;
		}

		static bool Contains(Limb limbAsMask, Limb limb) {
			return (limbAsMask & limb) == limb;
		}

		static Limb WithAdded(Limb limbAsMask, Limb limb)
		{
			return limbAsMask | limb;
		}

		static Limb WithRemoved(Limb limbAsMask, Limb limb)
		{
			return limbAsMask & ~limb;
		}

		static Limb FilterLegs(Limb limbAsMask)
		{
			return limbAsMask & BothLegs;
		}

		// Note: not implemented as operator<, as we don't want to necessarily compare single Limb values this way
		static bool IsLimbSetLessThan(Limb limbs1, Limb limbs2)
		{
			for (Limb limb : Limbs::Values)
			{
				if ((limbs1 & limb) == Limb::None && (limbs2 & limb) != Limb::None)
					return true;
				else if ((limbs1 & limb) != Limb::None && (limbs2 & limb) == Limb::None)
					return false;
			}

			return false;
		}

		static Limb GetPair(Limb limb)
		{
			switch (limb)
			{
				case Limb::LeftLeg: return Limb::RightLeg;
				case Limb::RightLeg: return Limb::LeftLeg;
				case Limb::LeftHand: return Limb::RightHand;
				case Limb::RightHand: return Limb::LeftHand;
				default: return Limb::None;
			}
		}

		static string_view GetCompactName(Limb limb)
		{
			switch (limb)
			{
				case Limb::LeftLeg: return "L";
				case Limb::RightLeg: return "R";
				case Limb::LeftHand: return "LH";
				case Limb::RightHand: return "RH";
				default: return "-";
			}
		}

		static Limb ParseCompactName(string_view sv)
		{
			if (sv == "L") return Limb::LeftLeg;
			if (sv == "R") return Limb::RightLeg;
			if (sv == "LH") return Limb::LeftHand;
			if (sv == "RH") return Limb::RightHand;
			if (sv == "LR") return BothLegs;
			return Limb::None;
		}
	};

	export ostream& operator<<(ostream& os, const Limb limbs)
	{
		switch (limbs)
		{
			case Limb::None:
			case Limb::LeftLeg:
			case Limb::RightLeg:
			case Limb::LeftHand:
			case Limb::RightHand:
				os << Limbs::GetCompactName(limbs);
				break;
			default:
				os << "[";
				bool first = true;
				for (Limb value : Limbs::Values)
				{
					if (Limbs::Contains(limbs, value))
					{
						if (!first)
							os << ", ";

						os << Limbs::GetCompactName(value);
						first = false;
					}
				}
				os << "]";
				break;
		}

		return os;
	}
}