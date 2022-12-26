export module LimbsOnPad;

import PadPanels;
import Limb;
import <initializer_list>;

export namespace DanceCommon
{
	export template<size_t rowSize>
	struct LimbsOnPad
	{
		PadPanels<Limb, rowSize> panels;

		LimbsOnPad() :
			panels{}
		{}

		LimbsOnPad(std::initializer_list<Limb> list)	:
			panels{list}
		{}
	};
}