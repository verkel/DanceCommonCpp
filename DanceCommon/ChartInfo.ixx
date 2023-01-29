export module ChartInfo;
import StdCore;
import PlayStyle;
import Difficulty;

namespace DanceCommon
{
	export struct ChartMatchInfo
	{
		static const ChartMatchInfo Any;

		optional<Difficulty> difficulty;
		optional<int> rating;
		optional<string> description;
	};

	export struct ChartInfo
	{
		Difficulty difficulty;
		int rating;
		string description;

		bool Matches(const ChartMatchInfo& matchInfo) const
		{
			if (matchInfo.difficulty && difficulty != matchInfo.difficulty)
				return false;

			if (matchInfo.rating && rating != matchInfo.rating)
				return false;

			if (matchInfo.description && description != matchInfo.description)
				return false;

			return true;
		}

		bool operator<(const ChartInfo& rhs) const
		{
			if (difficulty != rhs.difficulty)
				return difficulty < rhs.difficulty;

			if (rating != rhs.rating)
				return rating < rhs.rating;

			return description < rhs.description;
		}
	};

	const ChartMatchInfo ChartMatchInfo::Any{ nullopt, nullopt, nullopt };
}