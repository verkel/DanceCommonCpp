export module Limb;

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

	export class Limbs
	{
	public:
		static inline const Limb All = Limb::LeftLeg | Limb::RightLeg | Limb::LeftHand | Limb::RightHand;
	};
}