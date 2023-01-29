#include "Spider.h"
#include "world.h"

#include <RaspiLib.h>
#include <Utils.h>
#include <iostream>

// servos
#include <PCA9685.h>

// spider
#include "Leg.h"

#include <fstream>

#include <cmath>
const float PI = 3.14159265f; 

using namespace PiLib;
using namespace std;
using namespace SpiderLib;

Spider::Spider()
{ }

Spider::~Spider()
{ }

ErrorCode Spider::Init()
{
  bool isOk = RaspiLib::Startup();
  if (! isOk) return E_NOT_INIT;
 
  // create the two servo instances
  m_Servo0 = new PCA9685(0x41);
  m_Servo1 = new PCA9685(0x40);

  ErrorCode err = m_Servo0->Init();
  if (err != E_OK)
  {
    cleanup();
    return err;
  }

  err = m_Servo1->Init();
  if (err != E_OK)
  {
    cleanup();
    return err;
  }

  // set servo conditions
  m_Servo0->SetPWMFrequency(50);
  m_Servo1->SetPWMFrequency(50);

  // now create the legs
  m_rightFront = new Leg(15,14,13,false,this);
  m_rightMiddle = new Leg(12,11,10,false,this);
  m_rightBack = new Leg(9,8,31,false,this);

  m_leftFront = new Leg(16,17,18,true,this);
  m_leftMiddle = new Leg(19,20,21,true,this);
  m_leftBack = new Leg(22,23,27,true,this);

  // move legs to idle position - only this one direct
  m_leftFront->idlePosition();
  m_rightFront->idlePosition();
  m_leftBack->idlePosition();
  m_rightBack->idlePosition();
  m_leftMiddle->idlePosition();
  m_rightMiddle->idlePosition();

  err = InitLegs(); // set coordinates
  if (err != E_OK)
  {
    cleanup();
    return err;
  }

  // and move legs to idle position
  moveLegs(m_legCoordinates);

  return E_OK;
}

ErrorCode Spider::InitLegs()
{
  // we have 6 points
  m_legCoordinates.resize(6);

  // set leg ccordinates
  m_legCoordinates[RightFront] = RightFrontPosition;
  m_legCoordinates[RightMiddle] = RightMiddlePosition;
  m_legCoordinates[RightBack] = RightBackPosition;
  m_legCoordinates[LeftFront] = LeftFrontPosition;
  m_legCoordinates[LeftMiddle] = LeftMiddlePosition;
  m_legCoordinates[LeftBack] = LeftBackPosition;

  return E_OK;
}

ErrorCode Spider::Reset()
{
  return E_NOT_IMPLEMENTED;
}

ErrorCode Spider::StretchLegs()
{
  if (! m_Servo0) return E_NOT_INIT;

  // if servos are init, we also have the legs
  m_leftMiddle->stretch();
  m_rightMiddle->stretch();
  m_leftBack->stretch();
  m_rightBack->stretch();
  m_leftFront->stretch();
  m_rightFront->stretch();

  return E_OK;
}

ErrorCode Spider::LegsToIdle()
{
  if (! m_Servo0) return E_NOT_INIT;

  // TODO: this is only for testing, remove later!
  m_leftFront->idlePosition();
  m_rightFront->idlePosition();
  m_leftBack->idlePosition();
  m_rightBack->idlePosition();
  m_leftMiddle->idlePosition();
  m_rightMiddle->idlePosition();

  // make sure our internal coordinates match
  InitLegs();
  moveLegs(m_legCoordinates);

  // stand up
  Raise();
  moveLegs(m_legCoordinates);

  // maybe for later: check current position and move one leg after the other back to idle position
  // we may just end up in the middle of a step
  // and want to restore stable position without getting unstable

  return E_OK;
}

ErrorCode Spider::SetServoAngle(uint8_t channel, float angle)
{
  if (! m_Servo0) return E_NOT_INIT;

  ErrorCode err = E_OK;

  if (channel < 16)
    err = m_Servo0->SetServoAngle(channel, angle);
  else
    err = m_Servo1->SetServoAngle(channel-16, angle);

  return err;
}

void Spider::cleanup()
{
  delete m_Servo0;
  m_Servo0 = nullptr;
  delete m_Servo1;
  m_Servo1 = nullptr;
}

// the main loop
void Spider::work()
{

}

// start walking with default speed
PiLib::ErrorCode Spider::Walk(int speed)
{
  ErrorCode err = E_OK;

  return err;
}

// turn spider in clockwise direction, negative angle -> counterclockwise
PiLib::ErrorCode Spider::Turn(int angle)
{
  ErrorCode err = E_OK;

  return err;
}

void Spider::moveLegs(const PointArray &points)
{
  static int count = 0;

  // for now just output coordinates in csv file
  dumpCoordinates(count++, points);

  // transform m_legCoordinates to leg positions
  PointArray legPosition;
  legPosition.resize(6);

  WorldTransformation::getLegCoordinates(points, legPosition);

  // can only doi that if we have real legs ...
  if (!m_Servo0) return;

  // then set each leg to the new position
  m_leftFront->gotoLegPosition(legPosition[LeftFront]);
  m_rightFront->gotoLegPosition(legPosition[RightFront]);
  m_leftBack->gotoLegPosition(legPosition[LeftBack]);
  m_rightBack->gotoLegPosition(legPosition[RightBack]);
  m_leftMiddle->gotoLegPosition(legPosition[LeftMiddle]);
  m_rightMiddle->gotoLegPosition(legPosition[RightMiddle]);
}

ErrorCode Spider::Raise(float value_mm)
{
  // check z coordinates and set to negative raise value

  if (value_mm < - 80.) value_mm = -80.;
  if (value_mm > 80.) value_mm = 80.;

  for (int i=0; i<6; i++)
  {
    float x = m_legCoordinates[i].X();
    float y = m_legCoordinates[i].Y();
    
    m_legCoordinates[i] = Vector3D(x, y, - value_mm);
  }

  return E_OK;
}

/******************************
 * Move font right leg
 * ****************************/
ErrorCode Spider::waveLeg(int wave_counter,int wave_start,int wave_stop,int leg_id){
  
  PiLib::ErrorCode err;
  Leg *p_leg;
  
  //cout << "Leg index "<< leg_id<<"\n";
  
  switch(leg_id){
     case 0:
      p_leg = this->m_leftFront;
      break;
     case 1:
      p_leg = this->m_leftMiddle;
      break;
     case 2:
      p_leg = this->m_leftBack;
      break;
     case 3:
      p_leg = this->m_rightFront;
      break;
     case 4:
      p_leg = this->m_rightMiddle;
      break;
     case 5:
      p_leg = this->m_rightBack;
      break;
     default:
      cerr << "Could not wave as leg index was out of bounds!\n";
      return E_NOT_INIT;
      
    }
  
  for(int i = 0; i < wave_counter; i ++){
      err = p_leg->setAngle(100,-80,wave_start);
      Utils::delay_ms(100);    
      err = p_leg->setAngle(90,-80,wave_stop);
      Utils::delay_ms(100);
      err = p_leg->setAngle(100,-80,wave_start);
      Utils::delay_ms(100);
   }
   err = p_leg->idlePosition();
    
  return err;
}



// move the spider
// later: return false if stopped
bool Spider::run(float dx, float dy, int speed_ms, float angle, int gait)
{
  // allow maximum move 35 mm per cycle 
  float x = limit(dx, -35, 35);
  float y = limit(dy, -35, 35);

  int delay = 1;  // 1 ms minimum
  if (speed_ms > 64) delay = speed_ms / 64;

  int Z = 40;

  float z = (float)Z / (float)c_steps;

  // deep copy
  PointArray point = m_legCoordinates;
  
  // for a given angle we only change y
  if (angle != 0.) x = 0.;

  vector<XYType> xy;
  xy.resize(6);
  xy[0] = xy[1] = xy[2] = xy[3] = xy[4] = xy[5] = XYType(0.f, 0.f);

  for (int i=0; i<6; ++i)
  {
    float x1, y1;
    x1 = (point[i].X() * cos(angle / 180. * PI) + point[i].Y() * sin(angle / 180. * PI) - point[i].X() + x) / (float)c_steps;
    y1 = (-point[i].X() * sin(angle / 180. * PI) + point[i].Y() * cos(angle / 180. * PI) - point[i].Y() + y) / (float)c_steps;
    xy[i] = XYType(x1, y1);
  }

  if (x == 0. && y == 0. && angle == 0.)
  {
    moveLegs(m_legCoordinates);
    return true;
  }

  if (gait == 1)
  {
    for (int j = 0; j<c_steps; j++)
    {
      calcStep(point[RightFront], xy[RightFront], Z, RightFront, j);
      calcStep(point[RightMiddle], xy[RightMiddle], Z, RightMiddle, j);
      calcStep(point[RightBack], xy[RightBack], Z, RightBack, j);
      calcStep(point[LeftFront], xy[LeftFront], Z, LeftFront, j);
      calcStep(point[LeftMiddle], xy[LeftMiddle], Z, LeftMiddle, j);
      calcStep(point[LeftBack], xy[LeftBack], Z, LeftBack, j);

      moveLegs(point);
      // make sure we know where our legs are if we break
      m_legCoordinates = point;

      // maybe put all feet back to the floor as well
      if (checkObstacle()) return false;

      Utils::delay_ms(delay);
    }
  }

  else
  {
    int whichLeg[] = {LeftFront, RightBack, LeftMiddle, RightFront, RightMiddle, LeftBack};

    for (int i=0; i<6; i++)
    {
      for (int j=0; j<c_steps/6; j++)
      {
        calcStep2(point[RightFront], xy[RightFront], RightFront, j, whichLeg[i] == RightFront);
        calcStep2(point[RightMiddle], xy[RightMiddle], RightMiddle, j, whichLeg[i] == RightMiddle);
        calcStep2(point[RightBack], xy[RightBack], RightBack, j, whichLeg[i] == RightBack);
        calcStep2(point[LeftFront], xy[LeftFront], LeftFront, j, whichLeg[i] == LeftFront);
        calcStep2(point[LeftMiddle], xy[LeftMiddle], LeftMiddle, j, whichLeg[i] == LeftMiddle);
        calcStep2(point[LeftBack], xy[LeftBack], LeftBack, j, whichLeg[i] == LeftBack);

        moveLegs(point);
        // make sure we know where our legs are if we break
        m_legCoordinates = point;

        // maybe put all feet back to the floor as well
        if (checkObstacle()) return false;

        Utils::delay_ms(delay);
      }
    }
  }

  // if we did well we should end up in (almost) idle position

  return true;
}

void Spider::calcStep(Vector3D &pos, const XYType &delta, float Z, LegPosition leg, int part)
{
  // make sure we do not violate borders
  if (part < 0) part = 0;
  if (part >= c_steps) part = c_steps-1;

  float x = pos.X();
  float y = pos.Y();
  float z = pos.Z();

  bool isTriple1 = (leg == RightFront || leg == RightBack || leg == LeftMiddle);

  // movement happens in 8 categories
  if (part < c_steps / 8)
  {
    if (isTriple1)
    {
      x = x - 4*delta.first;
      y = y - 4*delta.second;
    }
    else
    {
      x = x + 8*delta.first;
      y = y + 8*delta.second;
      z = z + c_liftFoot/c_steps * 8;
    }
  }
  else if (part < 2*c_steps / 8)
  {
    if (isTriple1)
    {
      x = x - 4*delta.first;
      y = y - 4*delta.second;
    }
    else
    {
      z = z - c_liftFoot/c_steps * 8;
    }
  }
  else if (part < 3*c_steps / 8)
  {
    if (isTriple1)
    {
      z = z + c_liftFoot/c_steps * 8;
    }
    else
    {
      x = x - 4*delta.first;
      y = y - 4*delta.second;
    }
  }
  else if (part < 5*c_steps / 8)
  {
    if (isTriple1)
    {
      x = x + 8*delta.first;
      y = y + 8*delta.second;
    }
    else
    {
      x = x - 4*delta.first;
      y = y - 4*delta.second;
    }
  }
  else if (part < 6*c_steps / 8)
  {
    if (isTriple1)
    {
      z = z - c_liftFoot/c_steps * 8;
    }
    else
    {
      x = x - 4*delta.first;
      y = y - 4*delta.second;
    }
  }
  else if (part < 7*c_steps / 8)
  {
    if (isTriple1)
    {
      x = x - 4*delta.first;
      y = y - 4*delta.second;
    }
    else
    {
      z = z + c_liftFoot/c_steps * 8;
    }
  }
  else
  {
    if (isTriple1)
    {
      x = x - 4*delta.first;
      y = y - 4*delta.second;
    }
    else
    {
      x = x + 8*delta.first;
      y = y + 8*delta.second;

      z = z - c_liftFoot/c_steps * 8;
    }
  }

  pos = Vector3D(x, y, z);
}

void Spider::calcStep2(PiLib::Vector3D &pos, const XYType &delta, LegPosition leg, int part, bool other)
{
  if (part < 0) part = 0;
  if (part >= c_steps) part = c_steps-1;

  float x = pos.X();
  float y = pos.Y();
  float z = pos.Z();

  if (other)
  {
    if (part < c_steps/18)
    {
      z = z + 18* c_liftFoot/c_steps;
    }
    else if (part < c_steps/9)
    {
      x = x + 30*delta.first;
      y = y + 30*delta.second;
    }
    else if (part < c_steps / 6)
    {
      z = z - 18* c_liftFoot/c_steps;
    }
  }
  else
  {
    x = x - 2*delta.first;
    y = y - 2*delta.second;
  }

  pos = Vector3D(x, y, z);
}


bool Spider::checkObstacle()
{
  // later: check camera and ultrasound distance

  // no obstacle encountered
  return false;
}

void Spider::dumpCoordinates(int line, const PointArray &points)
{
  ofstream ofs("LegCoordinates.csv", (line > 0) ? ofstream::app : ofstream::trunc);

  ofs << line << ";" << points[RightFront].X() << ";" << points[RightFront].Y() << ";" << points[RightFront].Z() << ";" <<
      points[RightMiddle].X() << ";" << points[RightMiddle].Y() << ";" << points[RightMiddle].Z() << ";" <<
      points[RightBack].X() << ";" << points[RightBack].Y() << ";" << points[RightBack].Z() << ";" <<
      points[LeftFront].X() << ";" << points[LeftFront].Y() << ";" << points[LeftFront].Z() << ";" <<
      points[LeftMiddle].X() << ";" << points[LeftMiddle].Y() << ";" << points[LeftMiddle].Z() << ";" <<
      points[LeftBack].X() << ";" << points[LeftBack].Y() << ";" << points[LeftBack].Z() << endl;
  ofs.close();
}
