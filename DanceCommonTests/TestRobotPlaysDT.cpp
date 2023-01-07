// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
#include <filesystem> // doesn't work as a module

import StdCore;
import Song;
import Chart;
import ChartUtils;
import PlayStyle;
import Difficulty;
import PlanningBot;
import Play;

using namespace DanceCommon;
namespace fs = std::filesystem;

struct ChartEntry
{
	shared_ptr<Song> song;
	shared_ptr<SinglesChart> chart;
	string songPath;
};

class TestRobotPlaysDT : public testing::TestWithParam<ChartEntry>
{
};

static vector<ChartEntry> GetCharts()
{
	vector<ChartEntry> results;
	for (const auto& songDir : fs::directory_iterator{ "songs\\DanceTricks" })
	{
		if (songDir.is_directory())
		{
			for (const auto& file : fs::directory_iterator{ songDir.path() })
			{
				if (file.is_regular_file() && file.path().extension() == ".sm")
				{
					string path = file.path().string();
					ifstream stream{ path };
					auto song = Song::LoadAsSharedPtr(stream);
					const auto& charts = song->GetSinglesCharts();
					for (const auto& chart : charts)
					{
						results.push_back(ChartEntry{song, chart, path});
					}
				}
			}
		}
	}

	return results;
}

static SinglesPlay LoadPlay(const ChartEntry& entry)
{
	string playPath = ChartUtils::GetChartPlayPath(entry.songPath, *entry.song, *entry.chart);
	ifstream stream{playPath};

	if (!stream)
		throw logic_error("Could not construct path for play");

	return SinglesPlay{stream};
}

TEST_P(TestRobotPlaysDT, Chart)
{
	const auto& param = GetParam();
	auto& chart = param.chart;
	SinglesPlanningBot bot;
	bot.SetAllowDoublesteps(true);
	auto existingPlay = LoadPlay(param);
	auto newPlay = bot.Play(*chart);
	FAIL() << "TODO compare to expected play";
}

INSTANTIATE_TEST_CASE_P(
	TestRobotPlaysDT,
	TestRobotPlaysDT,
	testing::ValuesIn(GetCharts()),
	// Show readable names for charts
	// But this causes these warnings on test discovery, for every chart:
	// Warning: Could not find source location for test TestRobotPlaysDT/TestRobotPlaysDT.Chart/Bodybuilder_Verkel_Expert_9
	[](const testing::TestParamInfo<ChartEntry>& info)
	{
		auto& song = info.param.song;
		auto& chart = info.param.chart;
		string name = format("{} {} {} {}", song->GetMetadata().GetTitle(), chart->GetDescription(), Difficulties::GetName(chart->GetDifficulty()), chart->GetRating());
		std::ranges::replace(name, ' ', '_');
		name.erase(std::remove_if(name.begin(), name.end(), []( auto const& c ) -> bool { return c != '_' && !std::isalnum(c); }), name.end());
		return name;
	});