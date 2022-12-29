export module RobotPlay;

import Play;
import State;
import NotePos;

namespace DanceCommon
{
	export template<size_t rowSize>
	class RobotPlay : public Play<rowSize>
	{
		using TState = State<rowSize>;

	private:
		
	public:
		RobotPlay()
		{
		}

		size_t GetCount() override
		{
			return 0;
		}

		const TState& operator[](NotePos pos) const override
		{
			return States<rowSize>::GetDefault();
		}
	};
}
