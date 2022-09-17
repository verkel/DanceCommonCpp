export module NoteData;
import PlayStyle;

export namespace DanceCommon
{
	export class NoteData
	{
	private:
		PlayStyle style;

	public:
		NoteData(PlayStyle style)
		{

		}

		PlayStyle GetStyle() const
		{
			return style;
		}
	};
}