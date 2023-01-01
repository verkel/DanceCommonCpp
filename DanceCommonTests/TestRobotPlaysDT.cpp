// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
#include <filesystem> // doesn't work as a module

import StdCore;
import Song;
import Chart;
import PlayStyle;
import Difficulty;
import PlanningBot;

using namespace DanceCommon;
namespace fs = std::filesystem;
using TSongAndChart = pair<shared_ptr<Song>, shared_ptr<SinglesChart>>;

class TestRobotPlaysDT : public testing::TestWithParam<TSongAndChart>
{
};

static vector<TSongAndChart> GetCharts()
{
	vector<TSongAndChart> results;
	for (const auto& songDir : fs::directory_iterator{ "songs/DanceTricks" })
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
						results.push_back({ song, chart });
					}
				}
			}
		}
	}

	return results;
}

TEST_P(TestRobotPlaysDT, Chart)
{
	const auto& param = GetParam();
	auto& chart = param.second;
	SinglesPlanningBot bot;
	bot.SetAllowDoublesteps(true);
	auto play = bot.Play(*chart);
	FAIL() << "TODO compare to expected play";
}

INSTANTIATE_TEST_CASE_P(
	TestRobotPlaysDT,
	TestRobotPlaysDT,
	testing::ValuesIn(GetCharts()),
	// Show readable names for charts
	// But this causes these warnings on test discovery, for every chart:
	// Warning: Could not find source location for test TestRobotPlaysDT/TestRobotPlaysDT.Chart/Bodybuilder_Verkel_Expert_9
	[](const testing::TestParamInfo<TSongAndChart>& info)
	{
		auto& song = info.param.first;
		auto& chart = info.param.second;
		string name = format("{} {} {} {}", song->GetMetadata().GetTitle(), chart->GetDescription(), Difficulties::GetName(chart->GetDifficulty()), chart->GetRating());
		std::ranges::replace(name, ' ', '_');
		name.erase(std::remove_if(name.begin(), name.end(), []( auto const& c ) -> bool { return c != '_' && !std::isalnum(c); }), name.end());
		return name;
	});