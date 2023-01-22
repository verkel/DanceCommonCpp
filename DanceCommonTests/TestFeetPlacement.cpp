// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"

import StdCore;
import FeetPlacement;
import Computations;
import Panel;

using namespace DanceCommon;

static int GetAngle(Panel leftLegPanel, Panel rightLegPanel)
{
	return Computations::GetFeetPlacement(leftLegPanel, rightLegPanel).GetAngle();
}

TEST(FeetPlacement, AngleIsCorrect)
{
	// Check feet are correct
	EXPECT_EQ(FeetPlacements::L_R_.GetLeftLegPanel(), Panel::Left);
	EXPECT_EQ(FeetPlacements::L_R_.GetRightLegPanel(), Panel::Up);

	EXPECT_EQ(FeetPlacements::_R__.GetLeftLegPanel(), Panel::Center);
	EXPECT_EQ(FeetPlacements::_R__.GetRightLegPanel(), Panel::Down);

	// Check angles: spin around
	EXPECT_EQ(GetAngle(Panel::Left, Panel::Right), 0);
	EXPECT_EQ(GetAngle(Panel::Up, Panel::Right), 45);
	EXPECT_EQ(GetAngle(Panel::Up, Panel::Down), 90);
	EXPECT_EQ(GetAngle(Panel::Right, Panel::Down), 135);
	EXPECT_EQ(GetAngle(Panel::Right, Panel::Left), 180);
	EXPECT_EQ(GetAngle(Panel::Down, Panel::Left), -135);
	EXPECT_EQ(GetAngle(Panel::Down, Panel::Up), -90);
	EXPECT_EQ(GetAngle(Panel::Left, Panel::Up), -45);

	// Check angles: left leg on center
	EXPECT_EQ(GetAngle(Panel::Center, Panel::Right), 0);
	EXPECT_EQ(GetAngle(Panel::Center, Panel::Down), 90);
	EXPECT_EQ(GetAngle(Panel::Center, Panel::Left), 180);
	EXPECT_EQ(GetAngle(Panel::Center, Panel::Up), -90);

	// Check angles: right leg on center
	EXPECT_EQ(GetAngle(Panel::Left, Panel::Center), 0);
	EXPECT_EQ(GetAngle(Panel::Up, Panel::Center), 90);
	EXPECT_EQ(GetAngle(Panel::Right, Panel::Center), 180);
	EXPECT_EQ(GetAngle(Panel::Down, Panel::Center), -90);
}
