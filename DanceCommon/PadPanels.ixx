export module PadPanels;

import StdCore;
import NoteType;
import PlayStyle;
import Panel;

export namespace DanceCommon
{
	export template<typename TValue, size_t rowSize>
	class PadPanels
	{
	private:
		static constexpr int GetIndexOffset()
		{
			return PlayStyles::CenterPanelCount(PlayStyles::GetStyle(rowSize));
		}

		static constexpr int Size()
		{
			return rowSize + GetIndexOffset();
		}

		TValue values[Size()];

	public:
		constexpr PadPanels() :
			values {}
		{ }

		constexpr PadPanels(initializer_list<TValue> list) :
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

		static constexpr int First()
		{
			return 0 - GetIndexOffset();
		}

		static constexpr int End()
		{
			return PlayStyles::ButtonCount(PlayStyles::GetStyle(rowSize));
		}

		TValue& operator[](Panel panel)
		{
			return (*this)[Panels::Index(panel)];
		}

		TValue& operator[](int index)
		{
			return values[index + GetIndexOffset()];
		}

		const TValue& operator[](Panel panel) const
		{
			return (*this)[Panels::Index(panel)];
		}

		const TValue& operator[](int index) const
		{
			return values[index + GetIndexOffset()];
		}

		friend bool operator==(const PadPanels& lhs, const PadPanels& rhs)
		{
			for (size_t i = 0; i < Size(); i++)
			{
				if (lhs.values[i] != rhs.values[i])
					return false;
			}

			return true;
		}

		friend bool operator!=(const PadPanels& lhs, const PadPanels& rhs)
		{
			return !(lhs == rhs);
		}

		bool operator<(const PadPanels& rhs) const
		{
			for (size_t i = 0; i < Size()-1; i++)
			{
				if (values[i] != rhs.values[i])
					return values[i] < rhs.values[i];
			}

			return values[Size()-1] < rhs.values[Size()-1];
		}

		int GetHashCode() const
		{
			int hash = Size();
			for (size_t i = 0; i < Size(); i++)
			{
				hash = hash * 314159 + static_cast<int>(values[i]);
			}
			return hash;
		}
	};
}