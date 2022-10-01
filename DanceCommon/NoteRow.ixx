export module NoteRow;
import <string_view>;
import NoteType;
import PlayStyle;

export namespace DanceCommon
{
	export template<NoteRowSize rowSize>
	struct NoteRow
	{
		NoteType notes[rowSize];

		NoteRow() :
			notes {}
		{ }

		NoteRow(const std::string_view& lineView)
		{
			for (size_t i = 0; i < rowSize; i++)
			{
				notes[i] = NoteTypes::Get(lineView[i]);
			}
		}

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