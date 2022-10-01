export module PlayStyle;
import <array>;
import <string>;
import <stdexcept>;

export namespace DanceCommon
{
	export enum class PlayStyle
	{
		Single = 0,
		Double = 1,
	};

	// cannot use scoped enums as template parameters
	export enum NoteRowSize : size_t
	{
		NoteRowSizeSingle = 4,
		NoteRowSizeDouble = 8
	};

	export class PlayStyles
	{
	private:
		static constexpr std::array NamesByPlayStyle
		{
			"Single",
			"Double"
		};

	public:
		static int CenterPanelCount(PlayStyle style)
		{
			switch (style)
			{
			case PlayStyle::Single:
				return 1;
			case PlayStyle::Double:
				return 2;
			}
			throw std::range_error("style");
		}

		static int ButtonCount(PlayStyle style) {
			return CenterPanelCount(style) * 4;
		}

		static int PanelCount(PlayStyle style)
		{
			return CenterPanelCount(style) + ButtonCount(style);
		}

		static constexpr PlayStyle GetStyle(NoteRowSize size)
		{
			switch (size)
			{
				case NoteRowSizeSingle: return PlayStyle::Single;
				case NoteRowSizeDouble: return PlayStyle::Double;
			}

			throw std::range_error("size");
		}

		static const std::string GetName(PlayStyle style)
		{
			return NamesByPlayStyle[(int)style];
		}
	};
}