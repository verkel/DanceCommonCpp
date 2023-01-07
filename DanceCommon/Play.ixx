module;

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

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

		TState ParseState(const rapidjson::Value& obj)
		{
			return TState{
				TLimbsOnPads::Empty,
				Limb::LeftLeg,
				Limbs::BothLegs};
		}

		size_t GetCount()
		{
			return states.size();
		}

		const TState& operator[](NotePos pos) const
		{
			return states.at(pos);
		}

		TState& operator[](NotePos pos)
		{
			return states.at(pos);
		}
	};

	export using SinglesPlay = Play<NoteRowSize::Single>;
}
