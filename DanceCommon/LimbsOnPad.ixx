export module LimbsOnPad;

import PadPanels;
import NoteType;

export namespace DanceCommon
{
	export template<size_t rowSize>
	struct LimbsOnPad
	{
		PadPanels<NoteType, rowSize> panels;

		LimbsOnPad() :
			panels{}
		{}
	};
}