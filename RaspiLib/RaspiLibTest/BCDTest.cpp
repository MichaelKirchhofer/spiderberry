#include <Utils.h>
#include <catch.hpp>

TEST_CASE("valid_BCD_number_is_converted_to_binary","[BCDTest]")
{
  int bcdCode1 = 0x22;
  int bcdCode2 = 0x00;
  int bcdCode3 = 0x99;

  int result1 = Utils::fromBCD(bcdCode1);
  int result2 = Utils::fromBCD(bcdCode2);
  int result3 = Utils::fromBCD(bcdCode3);

  REQUIRE(result1 == 22);
  REQUIRE(result2 == 0);
  REQUIRE(result3 == 99);
}

TEST_CASE("BCD_numbers_greater_99_are_truncated","[BCDTest]")
{
  int bcdCode1 = 0x1100;
  int bcdCode2 = 0x0111;

  int result1 = Utils::fromBCD(bcdCode1);
  int result2 = Utils::fromBCD(bcdCode2);

  REQUIRE(result1 == 0);
  REQUIRE(result2 == 11);
}

TEST_CASE("int_less_than_100_can_be_converted_to_BCD","[BCDTest]")
{
  int value1 = 0;
  int value2 = 50;
  int value3 = 99;

  int result1 = Utils::toBCD(value1);
  int result2 = Utils::toBCD(value2);
  int result3 = Utils::toBCD(value3);

  REQUIRE(result1 == 0x00);
  REQUIRE(result2 == 0x50);
  REQUIRE(result3 == 0x99);
}

TEST_CASE("int_greater_99_are_truncated_in_BCD","[BCDTest]")
{
  int value1 = 100;
  int value2 = 1120;
  int value3 = 2399;

  int result1 = Utils::toBCD(value1);
  int result2 = Utils::toBCD(value2);
  int result3 = Utils::toBCD(value3);

  REQUIRE(result1 == 0x00);
  REQUIRE(result2 == 0x20);
  REQUIRE(result3 == 0x99);
}

TEST_CASE("int_less_than_zero_deliver_zero_BCD","[BCDTest]")
{
  int value1 = -1;
  int value2 = -99;

  int result1 = Utils::toBCD(value1);
  int result2 = Utils::toBCD(value2);

  REQUIRE(result1 == 0x00);
  REQUIRE(result2 == 0x00);
}