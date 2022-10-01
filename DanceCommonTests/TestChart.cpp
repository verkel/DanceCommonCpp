// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
import Chart;
import ChartInfo;
import PlayStyle;
import Difficulty;
import NoteLength;
import NotePos;
import NoteRow;
import <fstream>;
import <optional>;
import <string>;
import <string_view>;

using namespace DanceCommon;

TEST(Chart, Load_ExistingChart)
{
	std::ifstream stream{ "Vertex_Delta.sm" };
	SinglesChart chart{ stream, ChartMatchInfo{ PlayStyle::Single, Difficulty::Hard, std::nullopt, std::nullopt } };
	EXPECT_EQ(Difficulty::Hard, chart.GetDifficulty());
	EXPECT_EQ(9, chart.GetRating());
	EXPECT_EQ("Verkel", chart.GetDescription());
	EXPECT_EQ(18793, chart.GetLength());

	NotePos pos = NoteLength::Measure * 5 + NoteLength::Note_16th * 3;
	auto noteRow = chart.GetNoteRow(pos);
	EXPECT_EQ(NoteRow<NoteRowSizeSingle> { "M000" }, noteRow);
}