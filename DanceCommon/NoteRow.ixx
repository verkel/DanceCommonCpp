export module NoteRow;
import <string_view>;
import <ranges>;
import NoteType;
import PlayStyle;
import Panel;

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

		NoteType GetNote(size_t i) const
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

		Panel GetTappables() const
		{
			Panel result = Panel::None;
			for (int i = 0; i < rowSize; i++)
			{
				if (NoteTypes::IsTappable(notes[i]))
				{
					result = result | Panels::ForIndex(i);
				}
			}
			return result;
		}

		std::tuple<Panel, int> GetTappablesWithCount() const
		{
			Panel result = Panel::None;
			int count = 0;
			for (int i = 0; i < rowSize; i++)
			{
				if (NoteTypes::IsTappable(notes[i]))
				{
					result = result | Panels::ForIndex(i);
					count++;
				}
			}
			return std::tuple{result, count};
		}

		Panel GetHoldEnds() const
		{
			Panel result = Panel::None;
			for (int i = 0; i < rowSize; i++)
			{
				if (notes[i] == NoteType::HoldEnd)
				{
					result = result | Panels::ForIndex(i);
				}
			}
			return result;
		}

		template<typename TFunc>
		void ForEachHoldEnd(TFunc f) const
		{
			ForEachPanelWith(NoteType::HoldEnd, f);
		}

		template<typename TFunc>
		void ForEachMine(TFunc f) const
		{
			ForEachPanelWith(NoteType::Mine, f);
		}

		/*
		auto GetHoldEndsRange() const
		{
			return notes
				| std::views::filter([](const auto& nt) { return nt == NoteType::HoldEnd; });
		}

		auto GetMinesRange() const
		{
			return notes
				| std::views::filter([](const auto& nt) { return nt == NoteType::Mine; });
		}
		*/

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

	private:
		template<typename TFunc>
		void ForEachPanelWith(NoteType typeMask, TFunc f) const
		{
			for (int i = 0; i < rowSize; i++)
			{
				if (NoteTypes::Is(notes[i], typeMask))
				{
					f(static_cast<Panel>(i));
				}
			}
		}
	};

	export template<size_t rowSize>
	class NoteRows
	{
		static inline const NoteRow<rowSize> Empty = {};
	};

}