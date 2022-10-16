// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
import Song;
import Chart;
import ChartInfo;
import PlayStyle;
import Difficulty;
import NoteLength;
import NotePos;
import NoteRow;
import NoSuchChartException;
import ParseException;
import <fstream>;
import <optional>;
import <string>;
import <string_view>;
import <cmath>;

using namespace DanceCommon;

const double Epsilon = 0.00001;
const double TimeEpsilon = 0.001;

Song GetVertexDelta()
{
	std::ifstream stream{ "Vertex_Delta.sm" };
	return Song::Load(stream);
}

Song GetSilikon()
{
	std::ifstream stream{ "silikon.sm" };
	return Song::Load(stream);
}

TEST(Song, Load)
{
	Song song = GetVertexDelta();
	auto m = song.GetMetadata();
	EXPECT_EQ("Vertex Delta", m.GetTitle());
	EXPECT_EQ("Silvia", m.GetArtist());
	EXPECT_EQ("Steps by Verkel", m.GetCredit());
	EXPECT_EQ("Vertex_Delta-banner.png", m.GetBanner());
	EXPECT_EQ("Vertex_Delta-bg.jpg", m.GetBackground());
	EXPECT_EQ("cactuar.png", m.GetCdtitle());
	EXPECT_EQ("Vertex_Delta.ogg", m.GetMusic());
	EXPECT_NEAR(-0.847, m.GetOffset(), Epsilon);
	EXPECT_NEAR(53.103, m.GetSamplestart(), Epsilon);
	EXPECT_NEAR(12.000, m.GetSamplelength(), Epsilon);
	EXPECT_EQ("YES", m.GetSelectable());

	auto charts = song.GetSinglesCharts();
	EXPECT_EQ(5, charts.size());
	
	size_t i = 0;
	for (auto chart : charts)
	{
		EXPECT_EQ("Verkel", chart->GetDescription());

		switch (i)
		{
		case 0:
			EXPECT_EQ(1, chart->GetRating());
			EXPECT_EQ(Difficulty::Novice, chart->GetDifficulty());
			break;
		case 1:
			EXPECT_EQ(4, chart->GetRating());
			EXPECT_EQ(Difficulty::Easy, chart->GetDifficulty());
			break;
		case 2:
			EXPECT_EQ(8, chart->GetRating());
			EXPECT_EQ(Difficulty::Medium, chart->GetDifficulty());
			break;
		case 3:
			EXPECT_EQ(9, chart->GetRating());
			EXPECT_EQ(Difficulty::Hard, chart->GetDifficulty());
			break;
		case 4:
			EXPECT_EQ(11, chart->GetRating());
			EXPECT_EQ(Difficulty::Expert, chart->GetDifficulty());
			break;
		}

		i++;
	}
}

// Not implemented
/*
TEST(Song, ChartInfos)
{
	FAIL();
}
*/

TEST(Song, GetPosition_VertexDelta)
{
	Song song = GetVertexDelta();
	EXPECT_EQ(0, std::lround(song.GetPosition(0.847))); // song begins
	EXPECT_EQ(4044, std::lround(song.GetPosition(34.543))); // Past about 1/5 of the song
	EXPECT_EQ(9216, std::lround(song.GetPosition(77.637))); // begin slowdown
	EXPECT_EQ(9540, std::lround(song.GetPosition(83.036))); // mid-slowdown
	EXPECT_EQ(10824, std::lround(song.GetPosition(104.433))); // slowdown ends
	EXPECT_EQ(18204, std::lround(song.GetPosition(165.925))); // almost end of song
	EXPECT_EQ(18744, std::lround(song.GetPosition(170.424))); // final hold ends
	EXPECT_EQ(18816, std::lround(song.GetPosition(171.024))); // past song end, next measure
}

TEST(Song, GetPosition_Silikon)
{
	Song song = GetSilikon();
	EXPECT_EQ(0, std::lround(song.GetPosition(0.140))); // song begins
	EXPECT_EQ(2046, std::lround(song.GetPosition(34.297))); // Past about 1/5 of the song
	EXPECT_EQ(6192, std::lround(song.GetPosition(85.785))); // 2/3 of the song
	EXPECT_EQ(10572, std::lround(song.GetPosition(146.816))); // song ends with a mine wall
	EXPECT_EQ(11024, std::lround(song.GetPosition(149.361))); // random spot past the song end
}

TEST(Song, GetTime_VertexDelta)
{
	FAIL();
}

TEST(Song, GetTime_Silikon)
{
	FAIL();
}

// Not implemented
/*
TEST(Song, Save)
{
	FAIL();
}
*/