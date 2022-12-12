#include <catch.hpp>

#include <world.h>
#include "DummyCallback.h"

#include <string>
#include <iostream>

using namespace SpiderLib;
using namespace PiLib;
using namespace std;

TEST_CASE("leg4 base position","[TransformationTest]")
{
  Vector3D result = WorldTransformation::WorldToLeg(LeftFrontPosition, LeftFront);

  cout << "leg4: " << result.X() << " - " << result.Y() << " - " << result.Z() << endl;

  REQUIRE(compare_float(result.X(), 139.8f, 0.1f) == true);
  REQUIRE(compare_float(result.Y(), -0.4f, 0.1f) == true);
  REQUIRE(compare_float(result.Z(), -14.f, 0.1f) == true);
}

TEST_CASE("leg5 base position","[TransformationTest]")
{
  Vector3D result = WorldTransformation::WorldToLeg(LeftMiddlePosition, LeftMiddle);

  cout << "leg5: " << result.X() << " - " << result.Y() << " - " << result.Z() << endl;

  REQUIRE(compare_float(result.X(), 140.f, 0.1f) == true);
  REQUIRE(compare_float(result.Y(), 0.f, 0.1f) == true);
  REQUIRE(compare_float(result.Z(), -14.f, 0.1f) == true);
}

TEST_CASE("leg6 base position","[TransformationTest]")
{
  Vector3D result = WorldTransformation::WorldToLeg(LeftBackPosition, LeftBack);

  cout << "leg6: " << result.X() << " - " << result.Y() << " - " << result.Z() << endl;

  REQUIRE(compare_float(result.X(), 139.8f, 0.1f) == true);
  REQUIRE(compare_float(result.Y(), 0.4f, 0.1f) == true);
  REQUIRE(compare_float(result.Z(), -14.f, 0.1f) == true);
}

TEST_CASE("leg1 base position","[TransformationTest]")
{
  Vector3D result = WorldTransformation::WorldToLeg(RightFrontPosition, RightFront);

  cout << "leg1: " << result.X() << " - " << result.Y() << " - " << result.Z() << endl;

  REQUIRE(compare_float(result.X(), 139.8f, 0.1f) == true);
  REQUIRE(compare_float(result.Y(), 0.4f, 0.1f) == true);
  REQUIRE(compare_float(result.Z(), -14.f, 0.1f) == true);
}

TEST_CASE("leg2 base position","[TransformationTest]")
{
  Vector3D result = WorldTransformation::WorldToLeg(RightMiddlePosition, RightMiddle);

  cout << "leg2: " << result.X() << " - " << result.Y() << " - " << result.Z() << endl;

  REQUIRE(compare_float(result.X(), 140.f, 0.1f) == true);
  REQUIRE(compare_float(result.Y(), 0.f, 0.1f) == true);
  REQUIRE(compare_float(result.Z(), -14.f, 0.1f) == true);
}

TEST_CASE("leg3 base position","[TransformationTest]")
{
  Vector3D result = WorldTransformation::WorldToLeg(RightBackPosition, RightBack);

  cout << "leg3: " << result.X() << " - " << result.Y() << " - " << result.Z() << endl;

  REQUIRE(compare_float(result.X(), 139.8f, 0.1f) == true);
  REQUIRE(compare_float(result.Y(), -0.4f, 0.1f) == true);
  REQUIRE(compare_float(result.Z(), -14.f, 0.1f) == true);
}
