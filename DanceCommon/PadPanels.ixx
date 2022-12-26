export module PadPanels;
import <string_view>;
import <initializer_list>;
import NoteType;
import PlayStyle;

export namespace DanceCommon
{
	export template<typename TValue, size_t rowSize>
	struct PadPanels
	{
		TValue values[rowSize];

		PadPanels() :
			values {}
		{ }

		PadPanels(std::initializer_list<TValue> list) :
			values {}
		{
			size_t i = 0;
			for (auto value : list)
			{
				values[i] = value;
				i++;
			}
		}

		bool IsEmpty() const
		{
			for (size_t i = 0; i < rowSize; i++)
			{
				if (values[i] != NoteType::Empty)
					return false;
			}

			return true;
		}

		TValue operator[](size_t i)
		{
		   return values[i];
		}

		/*
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
		*/
	};
}