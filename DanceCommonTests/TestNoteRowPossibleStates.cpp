// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"
import <vector>;
import <ostream>;
import <sstream>;
import State;
import Chart;
import NoteRowPossibleStates;
using namespace DanceCommon;

static const std::string Expected1 =
	"[State { [  ] [L ] [  ] [R ], cost=0, angle=-45, dAngle=-45, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [  ] [R ] [  ] [L ], cost=6, angle=135, dAngle=135, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }]";

static const std::string Expected2 =
	"[State { [  ] [L ] [R ] [RH], cost=1, angle=-90, dAngle=-90, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [  ] [L ] [RH] [R ], cost=0, angle=-45, dAngle=-45, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [  ] [R ] [L ] [RH], cost=1, angle=90, dAngle=90, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [  ] [RH] [L ] [R ], cost=2, angle=45, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [  ] [R ] [LH] [L ], cost=8, angle=135, dAngle=135, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [  ] [LH] [R ] [L ], cost=6, angle=-135, dAngle=-135, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }]";

static const std::string Expected3 =
	"[State { [L ] [R ] [LH] [RH], cost=0, angle=45, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [L ] [LH] [R ] [RH], cost=4, angle=-45, dAngle=-45, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [L ] [RH] [LH] [R ], cost=2, angle=0, dAngle=0, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [R ] [L ] [RH] [LH], cost=10, angle=-135, dAngle=-135, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [LH] [L ] [R ] [RH], cost=2, angle=-90, dAngle=-90, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [LH] [L ] [RH] [R ], cost=0, angle=-45, dAngle=-45, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [R ] [RH] [L ] [LH], cost=6, angle=135, dAngle=135, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [LH] [R ] [L ] [RH], cost=2, angle=90, dAngle=90, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [LH] [RH] [L ] [R ], cost=4, angle=45, dAngle=45, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [R ] [LH] [RH] [L ], cost=18, angle=180, dAngle=180, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [RH] [R ] [LH] [L ], cost=10, angle=135, dAngle=135, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }, State { [RH] [LH] [R ] [L ], cost=6, angle=-135, dAngle=-135, dstep=False, airDstep=False, spin=False, movedLegs=2, llFreed=False, rlFreed=False }]";

static std::vector<std::shared_ptr<SinglesState>> GetStatesForRow(std::string_view noteStr)
{
	SinglesChart chart{};
	NoteRow<NoteRowSize::Single> row{noteStr};
	chart.SetNoteRow(0, row, true);
	auto parent = std::make_shared<std::map<NotePos, NoteRowPossibleStates<NoteRowSize::Single>>>();
	NoteRowPossibleStates<NoteRowSize::Single> states{0, -1, parent, chart, true};
	states.InsertPossibleStates();
	return std::vector(states.GetStates());
}

static std::string ToString(const std::vector<std::shared_ptr<SinglesState>>& states)
{
	std::stringstream ss;
	ss << "[";
	bool first = true;
	for (auto state : states)
	{
		if (!first)
		{
			ss << ", ";
		}
		ss << *state;
		first = false;
	}
	ss << "]";
    return ss.str();
}

TEST(NoteRowPossibleStates, InsertMultipleTapsStates)
{
	EXPECT_EQ(Expected1, ToString(GetStatesForRow("0101")));
	EXPECT_EQ(Expected2, ToString(GetStatesForRow("0111")));
	EXPECT_EQ(Expected3, ToString(GetStatesForRow("1111")));
}
