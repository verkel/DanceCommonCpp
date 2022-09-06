export module ChartInfo;
import <string>;
import PlayStyle;
import Difficulty;

export struct ChartInfo
{
	PlayStyle style;
	std::string description;
	Difficulty difficulty;
	double rating;

	ChartInfo(PlayStyle style, Difficulty difficulty) :
		style{ style },
		difficulty{ difficulty }
	{
	}
};