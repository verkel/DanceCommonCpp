export module PlayStyle;
import <array>;
import <string>;
import <stdexcept>;

export namespace DanceCommon
{
	enum class PlayStyle
	{
		Single = 0,
		Double = 1,
	};

	// cannot use scoped enums as template parameters
	/*export enum class NoteRowSize : size_t
	{
		Single = 4,
		Double = 8
	};*/

	export class NoteRowSize
	{
	public:
		static constexpr size_t Single = 4;
		static constexpr size_t Double = 8;
	};

	export class PlayStyles
	{
	private:
		static constexpr std::array<std::string_view, 2> NamesByPlayStyle
		{
			"Single",
			"Double"
		};

	public:
		static constexpr int CenterPanelCount(PlayStyle style)
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

		static constexpr int ButtonCount(PlayStyle style) {
			return CenterPanelCount(style) * 4;
		}

		static constexpr int PanelCount(PlayStyle style)
		{
			return CenterPanelCount(style) + ButtonCount(style);
		}

		static constexpr PlayStyle GetStyle(size_t size)
		{
			switch (size)
			{
				case NoteRowSize::Single: return PlayStyle::Single;
				case NoteRowSize::Double: return PlayStyle::Double;
			}

			throw std::range_error("size");
		}

		static std::string_view GetName(PlayStyle style)
		{
			return NamesByPlayStyle[static_cast<int>(style)];
		}
	};
}