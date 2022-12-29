// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"

import StdCore;
import NotePos;
import NoteLength;

using namespace DanceCommon;

// 4th note positions:
//
// --- 0
// --  48
// --  96
// --  144
// --- 192

TEST(NotePos, GetNoteLength)
{
	EXPECT_EQ(NoteLength::Measure, NotePositions::GetNoteLength(0));
	EXPECT_EQ(NoteLength::Note_24th, NotePositions::GetNoteLength(8));
	EXPECT_EQ(NoteLength::Note_16th, NotePositions::GetNoteLength(12));
	EXPECT_EQ(NoteLength::Note_12th, NotePositions::GetNoteLength(16));
	EXPECT_EQ(NoteLength::Note_8th, NotePositions::GetNoteLength(24));
	EXPECT_EQ(NoteLength::Note_12th, NotePositions::GetNoteLength(32));
	EXPECT_EQ(NoteLength::Note_16th, NotePositions::GetNoteLength(36));
	EXPECT_EQ(NoteLength::Note_24th, NotePositions::GetNoteLength(40));
	EXPECT_EQ(NoteLength::Note_4th, NotePositions::GetNoteLength(48));
	EXPECT_EQ(NoteLength::Note_8th, NotePositions::GetNoteLength(72));
	EXPECT_EQ(NoteLength::Note_4th, NotePositions::GetNoteLength(96));
	EXPECT_EQ(NoteLength::Note_8th, NotePositions::GetNoteLength(120));
	EXPECT_EQ(NoteLength::Note_4th, NotePositions::GetNoteLength(144));
	EXPECT_EQ(NoteLength::Measure, NotePositions::GetNoteLength(192));
	EXPECT_EQ(NoteLength::Note_192nd, NotePositions::GetNoteLength(193));
	EXPECT_EQ(NoteLength::Note_96th, NotePositions::GetNoteLength(194));
}

TEST(NotePos, Floor)
{
	EXPECT_EQ(48, NotePositions::Floor(48, NoteLength::Note_4th));
	EXPECT_EQ(96, NotePositions::Floor(110, NoteLength::Note_4th));
	EXPECT_EQ(96, NotePositions::Floor(143, NoteLength::Note_4th));
	EXPECT_EQ(144, NotePositions::Floor(144, NoteLength::Note_4th));
	EXPECT_EQ(144, NotePositions::Floor(191, NoteLength::Note_4th));
	EXPECT_EQ(192, NotePositions::Floor(192, NoteLength::Note_4th));
}

TEST(NotePos, Ceil)
{
	EXPECT_EQ(0, NotePositions::Ceil(0, NoteLength::Note_4th));
	EXPECT_EQ(48, NotePositions::Ceil(48, NoteLength::Note_4th));
	EXPECT_EQ(48, NotePositions::Ceil(48, NoteLength::Note_4th));
	EXPECT_EQ(144, NotePositions::Ceil(144, NoteLength::Note_4th));
	EXPECT_EQ(144, NotePositions::Ceil(144, NoteLength::Note_4th));
	EXPECT_EQ(144, NotePositions::Ceil(144, NoteLength::Note_4th));
	EXPECT_EQ(192, NotePositions::Ceil(192, NoteLength::Note_4th));
	EXPECT_EQ(192, NotePositions::Ceil(192, NoteLength::Note_4th));
}