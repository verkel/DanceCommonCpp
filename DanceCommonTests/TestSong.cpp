// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"

import StdCore;
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

using namespace DanceCommon;

static const double Epsilon = 0.00001;
static const double TimeEpsilon = 0.001;

static Song GetVertexDelta()
{
	ifstream stream{ "songs/Vertex_Delta.sm" };
	return Song::Load(stream);
}

static Song GetSilikon()
{
	ifstream stream{ "songs/silikon.sm" };
	return Song::Load(stream);
}

TEST(Song, Load)
{
	Song song = GetVertexDelta();
	const auto& m = song.GetMetadata();
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

	const auto& charts = song.GetSinglesCharts();
	EXPECT_EQ(5, charts.size());
	
	size_t i = 0;
	for (const auto& chart : charts)
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
	EXPECT_EQ(0, lround(song.GetPosition(0.847))); // song begins
	EXPECT_EQ(4044, lround(song.GetPosition(34.543))); // Past about 1/5 of the song
	EXPECT_EQ(9216, lround(song.GetPosition(77.637))); // begin slowdown
	EXPECT_EQ(9540, lround(song.GetPosition(83.036))); // mid-slowdown
	EXPECT_EQ(10824, lround(song.GetPosition(104.433))); // slowdown ends
	EXPECT_EQ(18204, lround(song.GetPosition(165.925))); // almost end of song
	EXPECT_EQ(18744, lround(song.GetPosition(170.424))); // final hold ends
	EXPECT_EQ(18816, lround(song.GetPosition(171.024))); // past song end, next measure
}

TEST(Song, GetPosition_Silikon)
{
	Song song = GetSilikon();
	EXPECT_EQ(0, lround(song.GetPosition(0.140))); // song begins
	EXPECT_EQ(2046, lround(song.GetPosition(34.297))); // Past about 1/5 of the song
	EXPECT_EQ(6192, lround(song.GetPosition(85.785))); // 2/3 of the song
	EXPECT_EQ(10572, lround(song.GetPosition(146.816))); // song ends with a mine wall
	EXPECT_EQ(11024, lround(song.GetPosition(149.361))); // random spot past the song end
}

TEST(Song, GetTime_VertexDelta)
{
	Song song = GetVertexDelta();
	EXPECT_NEAR(0.847, song.GetTime(0), TimeEpsilon); // song begins
	EXPECT_NEAR(34.543, song.GetTime(4044), TimeEpsilon); // Past about 1/5 of the song
	EXPECT_NEAR(77.637, song.GetTime(9216), TimeEpsilon); // begin slowdown
	EXPECT_NEAR(83.036, song.GetTime(9540), TimeEpsilon); // mid-slowdown
	EXPECT_NEAR(104.433, song.GetTime(10824), TimeEpsilon); // slowdown ends
	EXPECT_NEAR(165.925, song.GetTime(18204), TimeEpsilon); // almost end of song
	EXPECT_NEAR(170.424, song.GetTime(18744), TimeEpsilon); // final hold ends
	EXPECT_NEAR(171.024, song.GetTime(18816), TimeEpsilon); // past song end, next measure

	EXPECT_NEAR(song.GetTime(0), song.GetTime(0, false) - song.GetMetadata().GetOffset(), TimeEpsilon);

}

TEST(Song, GetTime_Silikon)
{
	Song song = GetSilikon();
	EXPECT_NEAR(0.140, song.GetTime(0), TimeEpsilon); // song begins
	EXPECT_NEAR(34.297, song.GetTime(2046), TimeEpsilon); // Past about 1/5 of the song
	EXPECT_NEAR(85.785, song.GetTime(6192), TimeEpsilon); // 2/3 of the song
	EXPECT_NEAR(146.816, song.GetTime(10572), TimeEpsilon); // song ends with a mine wall
	EXPECT_NEAR(149.361, song.GetTime(11024), TimeEpsilon); // random spot past the song end

	EXPECT_NEAR(song.GetTime(0), song.GetTime(0, false) - song.GetMetadata().GetOffset(), TimeEpsilon);
}

// Not implemented
/*
TEST(Song, Save)
{
	FAIL();
}
*/