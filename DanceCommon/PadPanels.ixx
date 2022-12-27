export module PadPanels;
import <string_view>;
import <initializer_list>;
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

		TValue values[rowSize + GetIndexOffset()];

	public:
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

		friend bool operator==(const PadPanels& lhs, const PadPanels& rhs)
		{
			for (size_t i = 0; i < rowSize; i++)
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
	};
}