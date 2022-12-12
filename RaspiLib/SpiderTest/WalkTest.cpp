#include <Utils.h>
#include <Spider.h>
#include <world.h>

#include <catch.hpp>

#include <string>
#include <iostream>

using namespace SpiderLib;
using namespace PiLib;
using namespace std;

TEST_CASE("init spider to idle position","[WalkTest]")
{
  Spider sp;

  ErrorCode err = sp.InitLegs();

  REQUIRE(err == E_OK);

  const PointArray &points = sp.LegCoordinates();

  REQUIRE(points[RightFront] == RightFrontPosition);
  REQUIRE(points[RightMiddle] == RightMiddlePosition);
  REQUIRE(points[RightBack] == RightBackPosition);
  REQUIRE(points[LeftFront] == LeftFrontPosition);
  REQUIRE(points[LeftMiddle] == LeftMiddlePosition);
  REQUIRE(points[LeftBack] == LeftBackPosition);
}

TEST_CASE("do one step cycle straight forward, gait 1","[WalkTest]")
{
  Spider sp;
  ErrorCode err = sp.InitLegs();
  err = sp.Raise();

  bool res = sp.run(0., 32.);
  res = sp.run(0.,32.);

  REQUIRE(res == true);

}

TEST_CASE("do one step cycle straight forward, gait 2","[WalkTest]")
{
  Spider sp;
  ErrorCode err = sp.InitLegs();
  err = sp.Raise();

  bool res = sp.run(0., 32., 1000, 0., 2);
  res = sp.run(0.,32., 1000, 0., 2);

  REQUIRE(res == true);

}