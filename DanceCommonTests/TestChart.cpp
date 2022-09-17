// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
import Chart;
import ChartInfo;
import PlayStyle;
import Difficulty;
import <fstream>;

using namespace DanceCommon;

TEST(Chart, Load_ExistingChart)
{
	std::ifstream stream{ "Vertex_Delta.sm" };
	Chart chart{ stream, ChartInfo{ PlayStyle::Single, Difficulty::Hard } };
	EXPECT_EQ(Difficulty::Hard, chart.GetDifficulty());
	EXPECT_EQ(9, chart.GetRating());
	EXPECT_EQ("Verkel", chart.GetDescription());
	EXPECT_EQ(18793, chart.GetLength());
}