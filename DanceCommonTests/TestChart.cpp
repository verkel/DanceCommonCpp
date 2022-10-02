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
import NoSuchChartException;
import <fstream>;
import <optional>;
import <string>;
import <string_view>;

using namespace DanceCommon;

SinglesChart GetVertexDeltaHard()
{
	std::ifstream stream{ "Vertex_Delta.sm" };
	return SinglesChart{ stream, ChartMatchInfo{ PlayStyle::Single, Difficulty::Hard, std::nullopt, std::nullopt } };
}

SinglesChart GetBadBoyExpert()
{
	std::ifstream stream{ "Bad Boy.sm" };
	return SinglesChart{ stream, ChartMatchInfo{ PlayStyle::Single, Difficulty::Expert, std::nullopt, std::nullopt } };
}


TEST(Chart, Load_ExistingChart)
{
	SinglesChart chart = GetVertexDeltaHard();
	EXPECT_EQ(Difficulty::Hard, chart.GetDifficulty());
	EXPECT_EQ(9, chart.GetRating());
	EXPECT_EQ("Verkel", chart.GetDescription());
	EXPECT_EQ(18793, chart.GetLength());

	NotePos pos = NoteLength::Measure * 5 + NoteLength::Note_16th * 3;
	auto noteRow = chart.GetNoteRow(pos);
	EXPECT_EQ(NoteRow<NoteRowSizeSingle> { "M000" }, noteRow);
}

TEST(Chart, Load_NonExistentChart)
{
	try
	{
		std::ifstream stream{ "Vertex_Delta.sm" };
		SinglesChart chart{ stream, ChartMatchInfo{ PlayStyle::Single, Difficulty::Novice, 42, std::nullopt } };
	}
	catch (NoSuchChartException&)
	{
		return;
	}

	FAIL() << "Reading non-existent chart did not throw exception properly";
}

TEST(Chart, GetNoteRow_IterateOverChart)
{
	SinglesChart chart = GetVertexDeltaHard();

	int count = 0;
	int nonEmptyCount = 0;

	for (NotePos position = 0; chart.Contains(position); position++)
	{
		auto noteRow = chart.GetNoteRow(position);
		
		if (!noteRow.IsEmpty())
			nonEmptyCount++;

		count++;
	}

	EXPECT_GT(nonEmptyCount, 0);
	EXPECT_EQ(18793, count);
}

TEST(Chart, GetNoteRow_IterateOverChart_SkipEmpty)
{
	SinglesChart chart = GetVertexDeltaHard();

	int count = 0;

	for (NotePos position = 0; chart.Contains(position); position = chart.NextPosition(position))
	{
		auto noteRow = chart.GetNoteRow(position);
		
		if (position != 0)
			EXPECT_FALSE(noteRow.IsEmpty());
		
		count++;
	}

	EXPECT_EQ(670, count);
}

TEST(Chart, GetNoteRow_IterateOverChart_Backwards)
{
	SinglesChart chart = GetVertexDeltaHard();

	int count = 0;

	for (NotePos position = chart.GetLastPosition(); chart.Contains(position); position--)
	{
		auto noteRow = chart.GetNoteRow(position);
		count++;
	}

	EXPECT_EQ(18745, count);
}

TEST(Chart, GetNoteRow_IterateOverChart_BackwardsSkipEmpty)
{
	SinglesChart chart = GetVertexDeltaHard();

	int count = 0;

	for (NotePos position = chart.GetLastPosition(); chart.Contains(position); 
		position = chart.PreviousPosition(position))
	{
		auto noteRow = chart.GetNoteRow(position);
		EXPECT_FALSE(noteRow.IsEmpty());
		count++;
	}

	EXPECT_EQ(669, count);
}

TEST(Chart, PreviousPosition)
{
	SinglesChart chart = GetBadBoyExpert();
	NotePos pos = chart.PreviousPosition(5928);
	EXPECT_LT(pos, 5928);
}