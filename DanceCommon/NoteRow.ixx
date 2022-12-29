export module NoteRow;

import StdCore;
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

		NoteRow(const string_view& lineView)
		{
			for (size_t i = 0; i < rowSize; i++)
			{
				notes[i] = NoteTypes::Get(lineView[i]);
			}
		}

		NoteType operator[](Panel panel) const
		{
			return (*this)[Panels::Index(panel)];
		}

		NoteType operator[](int index) const
		{
			return notes[index];
		}
		
		NoteType operator[](size_t index) const
		{
			return notes[index];
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
			return GetPanelsWith(NoteTypes::Tappable);
		}

		tuple<Panel, int> GetTappablesWithCount() const
		{
			return GetPanelsWithCount(NoteTypes::Tappable);
		}

		Panel GetFirstTappable() const
		{
			return GetFirstPanelWith(NoteTypes::Tappable);
		}

		Panel GetHoldables() const
		{
			return GetPanelsWith(NoteTypes::Holdable);
		}

		Panel GetHoldEnds() const
		{
			return GetPanelsWith(NoteType::HoldEnd);
		}

		template<typename TFunc>
		void ForEachTappable(TFunc f) const
		{
			ForEachPanelWith(NoteTypes::Tappable, f);
		}

		template<typename TFunc>
		void ForEachHoldable(TFunc f) const
		{
			ForEachPanelWith(NoteTypes::Holdable, f);
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
					f(Panels::ForIndex(i));
				}
			}
		}

		Panel GetPanelsWith(NoteType typeMask) const
		{
			Panel result = Panel::None;
			for (int i = 0; i < rowSize; i++)
			{
				if (NoteTypes::Is(notes[i], typeMask))
				{
					result = result | Panels::ForIndex(i);
				}
			}
			return result;
		}

		tuple<Panel, int> GetPanelsWithCount(NoteType typeMask) const
		{
			Panel result = Panel::None;
			int count = 0;
			for (int i = 0; i < rowSize; i++)
			{
				if (NoteTypes::Is(notes[i], typeMask))
				{
					result = result | Panels::ForIndex(i);
					count++;
				}
			}
			return tuple{result, count};
		}

		Panel GetFirstPanelWith(NoteType typeMask) const
		{
			for (int i = 0; i < rowSize; i++)
			{
				if (NoteTypes::Is(notes[i], typeMask))
				{
					return Panels::ForIndex(i);
				}
			}
			return Panel::None;
		}
	};

	export template<size_t rowSize>
	class NoteRows
	{
		static inline const NoteRow<rowSize> Empty = {};
	};

}