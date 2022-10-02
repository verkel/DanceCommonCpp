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
import <fstream>;
import <optional>;
import <string>;
import <string_view>;

using namespace DanceCommon;

const double Epsilon = 0.00001;
const double TimeEpsilon = 0.001;

Song GetVertexDelta()
{
	std::ifstream stream{ "Vertex_Delta.sm" };
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
}

TEST(Song, ChartInfos)
{
	FAIL();
}

TEST(Song, GetPosition_VertexDelta)
{
	FAIL();
}

TEST(Song, GetPosition_Silikon)
{
	FAIL();
}

TEST(Song, GetTime_VertexDelta)
{
	FAIL();
}

TEST(Song, GetTime_Silikon)
{
	FAIL();
}

TEST(Song, Save)
{
	FAIL();
}