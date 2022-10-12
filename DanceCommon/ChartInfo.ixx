export module ChartInfo;
import <string>;
import <optional>;
import PlayStyle;
import Difficulty;

namespace DanceCommon
{
	export struct ChartMatchInfo
	{
		static const ChartMatchInfo Any;

		std::optional<PlayStyle> style;
		std::optional<Difficulty> difficulty;
		std::optional<int> rating;
		std::optional<std::string> description;
	};

	export struct ChartInfo
	{
		PlayStyle style;
		Difficulty difficulty;
		int rating;
		std::string description;

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

	const ChartMatchInfo ChartMatchInfo::Any{ std::nullopt, std::nullopt, std::nullopt, std::nullopt };
}