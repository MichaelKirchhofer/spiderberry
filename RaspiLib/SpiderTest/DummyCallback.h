#pragma once

#include <SpiderServos.h>

#include <string>
#include <map>

#include <cmath>

// this is a stub to check the SpiderServo callbacks

class DummyCallback : public SpiderLib::SpiderServos
{
  public:
    DummyCallback(const std::string &text) : m_text(text) {}

    void addExpectation(uint8_t channel, float angle);
    PiLib::ErrorCode SetServoAngle(uint8_t channel, float angle);

  private:
    std::string m_text;
    std::map<uint8_t, float> m_expectations;
};

bool compare_float(float x, float y, float epsilon = 0.01f);
