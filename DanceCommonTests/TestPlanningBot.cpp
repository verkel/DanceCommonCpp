// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
import <vector>;
import <ostream>;
import <sstream>;
import <array>;
import <string>;
import <string_view>;
import Song;
import Chart;
import PlanningBot;
using namespace DanceCommon;

static SinglesChart GetSilikonExpert()
{
	std::ifstream stream{ "silikon.sm" };
	return SinglesChart{ stream, ChartMatchInfo{ PlayStyle::Single, Difficulty::Expert, std::nullopt, std::nullopt } };
}

TEST(PlanningBot, PlaySilikon)
{
	std::array<std::string, 10> Expected =
	{
		// 0..4
		"State { [  ] [  ] [  ] [  ], cost=0, angle=0, dAngle=0, dstep=False, airDstep=False, spin=False, movedLegs=0, llFreed=False, rlFreed=False }",
		"State { [L*] [  ] [  ] [  ], cost=0, angle=0, dAngle=0, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
		"State { [L ] [R*] [  ] [  ], cost=0, angle=45, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
		"State { [  ] [R ] [L*] [  ], cost=0, angle=90, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
		"State { [  ] [  ] [L ] [R*], cost=0, angle=45, dAngle=-45, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
		// 5..9
		"State { [L*] [  ] [R ] [  ], cost=0, angle=-45, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
		"State { [L ] [  ] [  ] {R*}, cost=0, angle=0, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
		"State { [  ] [  ] [L*] [R ], cost=0, angle=45, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
		"State { [  ] [R*] [L ] [  ], cost=0, angle=90, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
		"State { [L*] [R ] [  ] [  ], cost=0, angle=45, dAngle=-45, dstep=False, airDstep=False, spin=False, movedLegs=1, llFreed=False, rlFreed=False }",
	};

	SinglesChart chart = GetSilikonExpert();
	SinglesPlanningBot bot;
	bot.SetAllowDoublesteps(true);
	auto play = bot.Play(chart);

	EXPECT_EQ(play.GetCount(), 825);

	int i = 0;
	NotePos position = 0;
	
	for (; chart.Contains(position); position = chart.NextPosition(position))
	{
		if (i == 5)
			break;
		
		EXPECT_EQ(Expected[i], play[position].ToString());
		i++;
	}

	for (int n = 0; n < 300; n++)
	{
		position = chart.NextPosition(position);
	}
	
	for (; chart.Contains(position); position = chart.NextPosition(position))
	{
		if (i == 10)
			break;
		
		EXPECT_EQ(Expected[i], play[position].ToString());
		i++;
	}
	
	EXPECT_EQ(4248, position);
}
