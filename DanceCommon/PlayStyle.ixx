export module PlayStyle;

export namespace DanceCommon
{
	export enum class PlayStyle
	{
		None = -1,
		Any = 0,
		Single = 1,
		Double = 2,
	};

	export enum NoteRowSize : size_t
	{
		SizeSingle = 4,
		SizeDouble = 8
	};

	export class PlayStyles
	{
	public:
		static int CenterPanelCount(PlayStyle style)
		{
			return (int)style;
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
				case SizeSingle: return PlayStyle::Single;
				case SizeDouble: return PlayStyle::Double;
				default: return PlayStyle::Single;
			}
		}
	};
}