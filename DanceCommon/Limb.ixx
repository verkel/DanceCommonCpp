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

	inline Limb operator|(Limb a, Limb b)
	{
	    return static_cast<Limb>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline Limb operator&(Limb a, Limb b)
	{
	    return static_cast<Limb>(static_cast<int>(a) & static_cast<int>(b));
	}

	inline Limb operator~(Limb a)
	{
	    return static_cast<Limb>(~static_cast<int>(a));
	}

	export class Limbs
	{
	public:
		static inline const Limb BothLegs = Limb::LeftLeg | Limb::RightLeg;
		static inline const Limb All = Limb::LeftLeg | Limb::RightLeg | Limb::LeftHand | Limb::RightHand;

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
	};
}