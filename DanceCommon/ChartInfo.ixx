export module ChartInfo;
import StdCore;
import PlayStyle;
import Difficulty;

namespace DanceCommon
{
	export struct ChartMatchInfo
	{
		static const ChartMatchInfo Any;

		optional<PlayStyle> style;
		optional<Difficulty> difficulty;
		optional<int> rating;
		optional<string> description;
	};

	export struct ChartInfo
	{
		PlayStyle style;
		Difficulty difficulty;
		int rating;
		string description;

		bool Matches(const ChartMatchInfo& matchInfo)
		{
			if (matchInfo.style && style != matchInfo.style)
				return false;

			if (matchInfo.difficulty && difficulty != matchInfo.difficulty)
				return false;

			if (matchInfo.rating && rating != matchInfo.rating)
				return false;

			if (matchInfo.description && description != matchInfo.description)
				return false;

			return true;
		}
	};

	const ChartMatchInfo ChartMatchInfo::Any{ nullopt, nullopt, nullopt, nullopt };
}