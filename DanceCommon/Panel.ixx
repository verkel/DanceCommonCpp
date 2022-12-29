export module Panel;

import StdCore;
import PlayStyle;
import MathUtils;

export namespace DanceCommon
{
	export enum class Panel
	{
		None = 0,

		Center2 = 1 << 1,
		Center = 1 << 2,

		Left = 1 << 3,
		Down = 1 << 4,
		Up = 1 << 5,
		Right = 1 << 6,

		Left2 = 1 << 7,
		Down2 = 1 << 8,
		Up2 = 1 << 9,
		Right2 = 1 << 10
	};

	inline Panel operator|(Panel a, Panel b)
	{
	    return static_cast<Panel>(static_cast<int>(a) | static_cast<int>(b));
	}

	export class Panels
	{
	public:
		/**
		 * Get the index of the given target (panel)
		 */
		static int Index(Panel panel)
		{
			switch (panel)
			{
				case Panel::Center2: return -2;
				case Panel::Center: return -1;

				case Panel::Left: return 0;
				case Panel::Down: return 1;
				case Panel::Up: return 2;
				case Panel::Right: return 3;

				case Panel::Left2: return 4;
				case Panel::Down2: return 5;
				case Panel::Up2: return 6;
				case Panel::Right2: return 7;
			}

			throw range_error("panel");
		}

		/**
		 * Get a Panel for the given index
		 *
		 * @param index The index in which positions are lined up in a NoteRow, or -1
		 *           for the first center panel and -2 for the second
		 */
		static Panel ForIndex(int index)
		{
			switch (index)
			{
				case -2: return Panel::Center2;
				case -1: return Panel::Center;

				case 0: return Panel::Left;
				case 1: return Panel::Down;
				case 2: return Panel::Up;
				case 3: return Panel::Right;

				case 4: return Panel::Left2;
				case 5: return Panel::Down2;
				case 6: return Panel::Up2;
				case 7: return Panel::Right2;
			}

			throw range_error("index");
		}

		/**
		 * Get whether this panel belongs to single play style or to double play style
		 */
		static PlayStyle Style(Panel panel)
		{
			switch (panel)
			{
				case Panel::None: return PlayStyle::Single;

				case Panel::Center: return PlayStyle::Single;
				case Panel::Left: return PlayStyle::Single;
				case Panel::Down: return PlayStyle::Single;
				case Panel::Up: return PlayStyle::Single;
				case Panel::Right: return PlayStyle::Single;

				case Panel::Center2: return PlayStyle::Double;
				case Panel::Left2: return PlayStyle::Double;
				case Panel::Down2: return PlayStyle::Double;
				case Panel::Up2: return PlayStyle::Double;
				case Panel::Right2: return PlayStyle::Double;
			}

			throw range_error("panel");
		}

		/**
		 * Is this panel a button that we can press to register a step?
		 *
		 * @return true for all panels except CENTER, CENTER_2 and NONE
		 */
		static bool IsButton(Panel panel)
		{
			return panel != Panel::None && panel != Panel::Center && panel != Panel::Center2;
		}

		/**
		 * Returns false for the null-panel NONE, otherwise true
		 */
		static bool Exists(Panel panel)
		{
			return panel != Panel::None;
		}

		static int Angle(Panel panel)
		{
			switch (panel)
			{
				case Panel::None: return 0;

				case Panel::Center2: return 0;
				case Panel::Center: return 0;

				case Panel::Left: return -90;
				case Panel::Down: return 180;
				case Panel::Up: return 0;
				case Panel::Right: return 90;

				case Panel::Left2: return -90;
				case Panel::Down2: return 180;
				case Panel::Up2: return 0;
				case Panel::Right2: return 90;
			}

			throw range_error("panel");
		}

		// For bitmasks

		/**
		 * Get the number of elements (1-bits) in this bitmask
		 */
		static int CountElements(Panel panel)
		{
			return MathUtils::BitCount(static_cast<int>(panel));
		}

		/**
		 * Get the elements in this bitmask
		 */

		/*static IEnumerable<Panel> GetElements(Panel mask)
		{
			foreach (Panel p in PanelsDouble) {
				if ((mask & p) != 0) yield return p;
			}
		}

		static Panel GetFirstElement(this Panel mask)
		{
			foreach (Panel p in PanelsDouble)
			{
				if ((mask & p) != 0) return p;
			}
			return Panel::None;
		}*/
	};
}
