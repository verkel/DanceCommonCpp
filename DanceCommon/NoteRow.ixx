export module NoteRow;
import NoteType;
import PlayStyle;

export namespace DanceCommon
{
	export template<NoteRowSize rowSize>
	struct NoteRow
	{
		NoteType notes[rowSize];

		/*static constexpr size_t GetSize(PlayStyle style)
		{
			switch (style)
			{
				case PlayStyle::Single: return 4;
				case PlayStyle::Double: return 8;
				default: return 0;
			}
		}*/
	};
}