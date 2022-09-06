// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
import Chart;
import ChartInfo;
import PlayStyle;
import Difficulty;
import <fstream>;

TEST(Chart, Load_ExistingChart)
{
	std::ifstream stream{ "Vertex_Delta.sm" };
	Chart chart{ stream, ChartInfo{ PlayStyle::Single, Difficulty::Hard } };
	GTEST_FAIL();
}