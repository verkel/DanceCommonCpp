export module RobotPlay;

import StdCore;
import Play;
import State;
import StateLinksUtils;
import NotePos;
import NoteRowPossibleStates;

namespace DanceCommon
{
	export template<size_t rowSize>
	class RobotPlay : public Play<rowSize>
	{
		using TState = State<rowSize>;
		using TNoteRowPossibleStates = NoteRowPossibleStates<rowSize>;
		using TStateLinks = StateLinks<rowSize>;

	private:
		map<NotePos, TState> optimalPlay;
		int playTotalCost;

	public:
		RobotPlay(const map<NotePos, shared_ptr<TNoteRowPossibleStates>>& rowPossibleStates) :
			playTotalCost(-1)
		{
			/*
			if (rowPossibleStates.size() > 0)
			{
				auto it = rowPossibleStates.begin();

				NotePos position = it->first;
				shared_ptr<TNoteRowPossibleStates> row1PossibleStates = it->second;
				const TState& state = row1PossibleStates->GetCheapestState();
				const TStateLinks& stateLinks = row1PossibleStates->GetStateLinks(state);
				optimalPlay[position] = state;
				SortCheapestStateFirst(*row1PossibleStates, state);
				it++;

				for (; it != rowPossibleStates.end(); it++)
				{
					position = it->first;
					shared_ptr<TNoteRowPossibleStates> states = it->second;
					state = StateLinksUtils<rowSize>::GetCheapestChild(stateLinks, *states); // <-- pointer vs ref. cannot reassign refs.
					stateLinks = states->GetStateLinks(state);
					optimalPlay[position] = state;
					SortCheapestStateFirst(*states, state);
				}
			}
			*/
		}

		void SortCheapestStateFirst(TNoteRowPossibleStates& states, const TState& cheapest)
		{
			/*
			auto statesList = states.GetStates();
			if (statesList.size() < 2) return;
			auto it = std::find(statesList.begin(), statesList.end(), cheapest);
			if (it != statesList.end())
			{
				statesList.erase(it);
			}
			else
			{
				throw exception("Failed to reorder states");
			}
			statesList.insert(statesList.begin(), cheapest);
			*/

			// TODO mismatch between pointers and inplace State. Rethink how to do this.
		}

		size_t GetCount() override
		{
			return optimalPlay.size();
		}

		const TState& operator[](NotePos pos) const override
		{
			return States<rowSize>::GetDefault();
		}
	};
}
