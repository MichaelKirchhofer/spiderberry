#include <Utils.h>
#include <Leg.h>
#include <catch.hpp>

#include <string>
#include <iostream>

#include "DummyCallback.h"

using namespace SpiderLib;
using namespace PiLib;
using namespace std;

TEST_CASE("get a leg with empty callback","[LegTest]")
{
  // 
  Leg rightLeg(1, 2, 3, false, nullptr);

  ErrorCode err = rightLeg.stretch();
  REQUIRE(err != E_OK);

  const Vector3D &angles = rightLeg.getAngles();

  REQUIRE(angles.X() == 0.);
  REQUIRE(angles.Y() == 0.);
  REQUIRE(angles.Z() == 0.);
}

TEST_CASE("get a left leg and stretch","[LegTest]")
{
  DummyCallback leftCallback("left leg stretch");
  leftCallback.addExpectation(4, 90.);
  leftCallback.addExpectation(5, 90.);
  leftCallback.addExpectation(6, 180.);

  Leg leftLeg(4,5,6,true,&leftCallback);

  ErrorCode err = leftLeg.stretch();
  REQUIRE(err == E_OK);

  const Vector3D &angles = leftLeg.getAngles();

  REQUIRE(angles.X() == 0.);
  REQUIRE(angles.Y() == 0.);
  REQUIRE(angles.Z() == 0.);
}

TEST_CASE("get a right leg and stretch","[LegTest]")
{
  DummyCallback rightCallback("right leg stretch");

  Leg rightLeg(1,2,3,false,&rightCallback);
  rightCallback.addExpectation(1, 90.);
  rightCallback.addExpectation(2, 90.);
  rightCallback.addExpectation(3, 0.);

  ErrorCode err = rightLeg.stretch();
  REQUIRE(err == E_OK);

  const Vector3D &angles = rightLeg.getAngles();

  REQUIRE(angles.X() == 0.);
  REQUIRE(angles.Y() == 0.);
  REQUIRE(angles.Z() == 0.);
}

// check transformation first ...

TEST_CASE("left leg transform idle position to angle","[LegTransformationTest]")
{
  // transformation does not need callback
  Leg leftLeg(4,5,6,true,nullptr);

  Vector3D idlePosition(140., 0., 0.);

  Vector3D angles = leftLeg.legPositionToAngles(idlePosition);

  REQUIRE(angles.X() == 90.);
  REQUIRE(compare_float(angles.Y(), -67.25f) == true);
  REQUIRE(compare_float(angles.Z(), 116.23f) == true);
}

TEST_CASE("right leg transform idle position to angle","[LegTransformationTest]")
{
  // transformation does not need callback
  Leg rightLeg(1,2,3,false,nullptr);

  Vector3D idlePosition(140., 0., 0.);

  Vector3D angles = rightLeg.legPositionToAngles(idlePosition);

  REQUIRE(angles.X() == 90.);
  REQUIRE(compare_float(angles.Y(), -67.25f) == true);
  REQUIRE(compare_float(angles.Z(), 116.23f) == true);
}

TEST_CASE("left leg transform raised position to angle","[LegTransformationTest]")
{
  // transformation does not need callback
  Leg leftLeg(4,5,6,true,nullptr);

  // raise spider, should not affect inner angle
  Vector3D legPosition(140., 0., 25.);

  Vector3D angles = leftLeg.legPositionToAngles(legPosition);

  REQUIRE(angles.X() == 90.);
  REQUIRE(compare_float(angles.Y(), -52.76f) == true);
  REQUIRE(compare_float(angles.Z(), 114.23f) == true);
}

TEST_CASE("left leg transform raised position 2 to angle","[LegTransformationTest]")
{
  // transformation does not need callback
  Leg leftLeg(4,5,6,true,nullptr);

  // raise spider, should not affect inner angle
  Vector3D legPosition(140., 30., 25.);

  Vector3D angles = leftLeg.legPositionToAngles(legPosition);

  REQUIRE(compare_float(angles.X(), 77.91f) == true);
  REQUIRE(compare_float(angles.Y(), -51.69f) == true);
  REQUIRE(compare_float(angles.Z(), 112.06f) == true);
}

// TODO: add a test where legs are pointing forward, check middle angle in callbacks

// from now on we check only one leg
// the difference in angles is only in the callback
TEST_CASE("set leg to idle position","[LegTest]")
{
  DummyCallback cb("right leg in idle position");

  Leg rightLeg(1,2,3,false,&cb);

  cb.addExpectation(1, 90.f);
  cb.addExpectation(2, 90.f + 67.25f);  // right is 90 - angle (-67,25)
  cb.addExpectation(3, 116.23f);

  ErrorCode err = rightLeg.idlePosition();
  REQUIRE(err == E_OK);

  const Vector3D &pos = rightLeg.getLegPosition();

  REQUIRE(compare_float(pos.X(), 140.f) == true);
  REQUIRE(compare_float(pos.Y(), 0.f) == true);
  REQUIRE(compare_float(pos.Z(), 0.f) == true);

  const Vector3D &angles = rightLeg.getAngles();

  REQUIRE(compare_float(angles.X(), 90.f) == true);
  REQUIRE(compare_float(angles.Y(), -67.25f) == true);
  REQUIRE(compare_float(angles.Z(), 116.23f) == true);
}