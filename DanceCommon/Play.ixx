module;

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

export module Play;
import StdCore;
import State;
import NotePos;
import Limb;
import PlayStyle;
import LimbsOnPad;
import StringUtils;
import ParseException;

namespace DanceCommon
{
	export template<size_t rowSize>
	class Play
	{
		using TState = State<rowSize>;
		using TStates = States<rowSize>;
		using TLimbsOnPad = LimbsOnPad<rowSize>;
		using TLimbsOnPads = LimbsOnPads<rowSize>;

		map<NotePos, TState> states;

	public:
		Play() {}

		Play(istream& stream)
		{
			rapidjson::IStreamWrapper isw{stream};
			rapidjson::Document doc;
			doc.ParseStream(isw);

			const auto& jsonStates = doc["states"];

			for (rapidjson::Value::ConstMemberIterator it = jsonStates.MemberBegin(); it != jsonStates.MemberEnd(); ++it)
			{
				int pos;
				if (!StringUtils::TryParseInt(it->name.GetString(), pos))
					throw ParseException("Cannot parse position key");

				states.insert({pos, ParseState(it->value)});
			}
		}

		// Disallow copy
		Play(const Play& other) = delete;
		Play& operator=(const Play& other) = delete;

		// Implement move
		Play(Play&& other) noexcept
			: states{std::move(other.states)}
		{
		}

		Play& operator=(Play&& other) noexcept
		{
			if (this == &other)
				return *this;
			states = std::move(other.states);
			return *this;
		}

		TState ParseState(const rapidjson::Value& obj)
		{
			const auto& occupiedPanelsJson = obj["occupiedPanels"];
			TLimbsOnPad occupiedPanels;
			int i = -TLimbsOnPad::GetIndexOffset();
			for (auto& limb : occupiedPanelsJson.GetArray())
			{
				occupiedPanels[i] = Limbs::ParseCompactName(limb.GetString());
				i++;
			}

			const auto& lastLegJson = obj["lastLeg"];
			Limb lastLeg = Limbs::ParseCompactName(lastLegJson.GetString());

			const auto& freeLimbsJson = obj["freeLimbs"];
			Limb freeLimbs = Limb::None;
			for (auto& limb : freeLimbsJson.GetArray())
			{
				freeLimbs = freeLimbs | Limbs::ParseCompactName(limb.GetString());
			}

			const auto& leftLegFreedJson = obj["leftLegFreed"];
			bool leftLegFreed = leftLegFreedJson.GetBool();
			
			const auto& rightLegFreedJson = obj["rightLegFreed"];
			bool rightLegFreed = rightLegFreedJson.GetBool();

			const auto& movedLegsAmountJson = obj["movedLegsAmount"];
			int movedLegsAmount = movedLegsAmountJson.GetInt();
			
			const auto& angleDeltaJson = obj["angleDelta"];
			int angleDelta = angleDeltaJson.GetInt();

			const auto& doublestepJson = obj["doublestep"];
			bool doublestep = doublestepJson.GetBool();

			const auto& airDoublestepJson = obj["airDoublestep"];
			bool airDoublestep = airDoublestepJson.GetBool();
			
			const auto& spinJson = obj["spin"];
			bool spin = spinJson.GetBool();

			return TState
			{
				occupiedPanels,
				lastLeg,
				freeLimbs,
				doublestep,
				airDoublestep,
				static_cast<unsigned char>(movedLegsAmount),
				static_cast<short>(angleDelta),
				spin,
				leftLegFreed,
				rightLegFreed
			};
		}

		size_t GetCount()
		{
			return states.size();
		}
		
		bool Contains(NotePos pos) const
		{
			return states.contains(pos);
		}

		const TState& Get(NotePos pos) const
		{
			return states.at(pos);
		}

		TState& operator[](NotePos pos)
		{
			return states[pos];
		}

		friend bool operator==(const Play& lhs, const Play& rhs)
		{
			return lhs.states == rhs.states;
		}

		friend bool operator!=(const Play& lhs, const Play& rhs)
		{
			return !(lhs == rhs);
		}

		void Diff(const Play& other, ostream& stream) const
		{
			for (auto& [pos, state] : states)
			{
				bool stateExistsInOther = other.Contains(pos);
				bool ok = stateExistsInOther && (state == other.Get(pos));
				if (!ok)
				{
					stream << "Measure " << NotePositions::ToMeasures(pos) << ":" << std::endl;
					if (!stateExistsInOther)
					{
						stream << "\tNo state at this measure in the second play" << std::endl;
					}
					else /* states are not equal */
					{
						state.Diff(other.Get(pos), stream);
					}
				}
			}
		}
	};

	export using SinglesPlay = Play<NoteRowSize::Single>;
}
