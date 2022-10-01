// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
import Chart;
import ChartInfo;
import PlayStyle;
import Difficulty;
import <fstream>;
import <optional>;

using namespace DanceCommon;

TEST(Chart, Load_ExistingChart)
{
	std::ifstream stream{ "Vertex_Delta.sm" };
	SinglesChart chart{ stream, ChartMatchInfo{ PlayStyle::Single, Difficulty::Hard, std::nullopt, std::nullopt } };
	EXPECT_EQ(Difficulty::Hard, chart.GetDifficulty());
	EXPECT_EQ(9, chart.GetRating());
	EXPECT_EQ("Verkel", chart.GetDescription());
	EXPECT_EQ(18793, chart.GetLength());
}