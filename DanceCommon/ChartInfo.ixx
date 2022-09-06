export module ChartInfo;
import <string>;
import PlayStyle;
import Difficulty;

export struct ChartInfo
{
	PlayStyle Style;
	std::string Description;
	Difficulty Difficulty;
	double Rating;
};