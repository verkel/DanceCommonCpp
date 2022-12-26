export module NoteRow;
import <string_view>;
import NoteType;
import PlayStyle;

export namespace DanceCommon
{
	export template<size_t rowSize>
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

		inline NoteType GetNote(size_t i) const
		{
			return notes[i];
		}

		bool IsEmpty() const
		{
			for (size_t i = 0; i < rowSize; i++)
			{
				if (notes[i] != NoteType::Empty)
					return false;
			}

			return true;
		}

		bool operator==(const NoteRow& rhs) const
		{
			for (size_t i = 0; i < rowSize; i++)
			{
				if (notes[i] != rhs.notes[i])
					return false;
			}

			return true;
		}

		bool operator!=(const NoteRow& rhs) const
		{
			return !operator==(rhs);
		}
	};

	export template<size_t rowSize>
	class NoteRows
	{
		static inline const NoteRow<rowSize> Empty = {};
	};

}