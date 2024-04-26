//
// ECE 579 (Spring 2024)
// Project
//
// Authors:
//     Alan Manuel Loreto Cornidez (aloretocornidez)
//     Elliot Zeurcher (ezuercher)
//     Eric Teitelbaum (ericteitelbaum)
//     Ted Ha (ttha)
//

#include <cstdio>
#include <limits>
#include <stdexcept>

#include "Edge.h"

namespace graph
{

PathDatabase dummypath;
TrafficDatabase dummytraffic;
CatTranDatabase dummycattran;

Edge::Edge()
    : src_(nullptr)
    , dest_(nullptr)
    , mode_(TransportMode::UNKNOWN)
    , distMile_(0.0)
    , speedMph_(0.0)
    , baseTimeMin_(0.0)
    , path_(dummypath)
    , traffic_(dummytraffic)
    , cattran_(dummycattran)
{
}

Edge::Edge(Node *src, Node *dest, TransportMode mode, PathDatabase &pdb, TrafficDatabase &tdb, CatTranDatabase &cdb)
    : src_(src)
    , dest_(dest)
    , mode_(mode)
    , distMile_(0.0)
    , speedMph_(0.0)
    , baseTimeMin_(0.0)
    , path_(pdb)
    , traffic_(tdb)
    , cattran_(cdb)
{
  switch (mode_)
  {
  case TransportMode::WALK:
    speedMph_ = WALK_KPH * KM_TO_MILE;
    break;
  case TransportMode::BIKE:
    speedMph_ = BIKE_KPH * KM_TO_MILE;
    break;
  case TransportMode::CATTRAN:
    speedMph_ = CATTRAN_KPH * KM_TO_MILE;
    break;
  case TransportMode::NONE:
  case TransportMode::UNKNOWN:
  default:
    speedMph_ = 0.0;
    printf("ERROR: (Edge::Edge) Bad mode of transportation!\n");
  }

  try
  {
    unsigned int id1 = (src_->id() < dest_->id()) ? (src_->id()) : (dest_->id());
    unsigned int id2 = (src_->id() < dest_->id()) ? (dest_->id()) : (src_->id());

    PathDatabase::PathDatabaseType &p = path_.db_.at(std::make_pair(id1, id2));
    distMile_ = p.dist_mile;

    if (0 != speedMph_)
    {
      baseTimeMin_ = distMile_ / speedMph_ * HR_TO_MIN;
    }
    else
    {
      printf("ERROR: (Edge::Edge) Speed cannot be zero!\n");
    }
  }
  catch (const std::out_of_range &oor)
  {
    (void)oor;
    printf("ERROR: (Edge::Edge) Path does not exist!\n");
  }
}

Edge::~Edge() {}

Edge::Edge(const Edge &copy)
    : path_(copy.path_)
    , traffic_(copy.traffic_)
    , cattran_(copy.cattran_)
{
  if (&copy != this)
  {
    this->src_ = copy.src_;
    this->dest_ = copy.dest_;
    this->mode_ = copy.mode_;
    this->distMile_ = copy.distMile_;
    this->speedMph_ = copy.speedMph_;
    this->baseTimeMin_ = copy.baseTimeMin_;
  }
}

Edge &Edge::operator=(const Edge &rhs)
{
  if (&rhs != this)
  {
    this->src_ = rhs.src_;
    this->dest_ = rhs.dest_;
    this->mode_ = rhs.mode_;
    this->distMile_ = rhs.distMile_;
    this->speedMph_ = rhs.speedMph_;
    this->baseTimeMin_ = rhs.baseTimeMin_;
    this->path_ = rhs.path_;
    this->traffic_ = rhs.traffic_;
    this->cattran_ = rhs.cattran_;
  }

  return *this;
}

Node *Edge::src() { return src_; }

Node *Edge::dest() { return dest_; }

double Edge::cost(CostType costType, unsigned int startHr, double startMin, unsigned int &endHr, double &endMin, EdgeStatus &status)
{
  double cost(std::numeric_limits<double>::max());

  switch (costType)
  {
  case CostType::DISTANCE:
    cost = distCost(status);
    break;
  case CostType::TIME_NO_TRAFFIC:
    cost = noTrafficCost(status);
    addMin(cost, startHr, startMin, endHr, endMin);
    break;
  case CostType::TIME_WITH_TRAFFIC:
    cost = trafficCost(startHr, startMin, status);
    addMin(cost, startHr, startMin, endHr, endMin);
    break;
  case CostType::UNKNOWN:
  default:
    status = EdgeStatus::EDGE_INVALID_COST_TYPE;
  }

  return cost;
}

double Edge::distCost(EdgeStatus &status)
{
  status = EdgeStatus::EDGE_OK;
  return distMile_;
}

double Edge::noTrafficCost(EdgeStatus &status)
{
  status = EdgeStatus::EDGE_OK;
  return baseTimeMin_;
}

double Edge::trafficCost(unsigned int hr, double min, EdgeStatus &status)
{
  status = EdgeStatus::UNKNOWN;
  double time(std::numeric_limits<double>::max());

  // Round to the nearest integer
  unsigned int intMin(static_cast<unsigned int>(min + 0.5));
  if (60 == intMin)
  {
    ++hr;
    intMin = 0;
  }

  double trafficCoeff(trafficCoefficient(hr, intMin, status));
  if (EdgeStatus::EDGE_OK == status)
  {
    double timeMin = distMile_ / (speedMph_ * trafficCoeff) * HR_TO_MIN;

    TransportMode sMode(src_->mode());
    TransportMode dMode(dest_->mode());

    // No transportation change
    if (sMode == dMode)
    {
      time = timeMin;

      status = EdgeStatus::EDGE_OK;
    }

    // If switching to CatTran, add wait time
    // Note: Bike -> CatTran assumes enough time to lock up the bike
    else if (TransportMode::CATTRAN == dMode)
    {
      try
      {
        struct CatTranDatabase::CatTranDatabaseType &c = cattran_.db_.at(std::make_tuple(src_->id(), hr, intMin));

        time = timeMin + c.minToNext;

        status = EdgeStatus::EDGE_OK;
      }
      catch (const std::out_of_range &oor)
      {
        (void)oor;
        status = EdgeStatus::EDGE_NOT_IN_CATTRAN_DB;
        printf("ERROR: (Edge::getTrafficCost) Node/hr/min do not in the CatTran DB!\n");
      }
    }

    // If switching to Bike, add time to lock/unlock bike
    else if ((TransportMode::BIKE == sMode) || (TransportMode::BIKE == dMode))
    {
      time = timeMin + 1;

      status = EdgeStatus::EDGE_OK;
    }

    // Switch from CatTran to walk, or None to walk, does not cost more time
    else
    {
      time = timeMin;

      status = EdgeStatus::EDGE_OK;
    }
  }

  return time;
}

double Edge::trafficCoefficient(unsigned int hr, unsigned int min, EdgeStatus &status)
{
  double coeff(1.0);

  try
  {
    struct TrafficDatabase::TrafficDatabaseType &t = traffic_.db_.at(std::make_pair(hr, min));
    coeff *= t.coefficient;
    status = EdgeStatus::EDGE_OK;
  }
  catch (const std::out_of_range &oor)
  {
    (void)oor;
    status = EdgeStatus::EDGE_NOT_IN_TRAFFIC_DB;
    printf("ERROR: (Edge::getTrafficCoefficient) hr/min do not in the Traffic DB!\n");
  }

  return coeff;
}

void Edge::print()
{
  printf("%2d%c -> %2d%c (%s): ", src_->id(), tModeChar(src_->mode()), dest_->id(), tModeChar(dest_->mode()), tModeStr(mode_));
  printf("mile = %f, ", distMile_);
  printf("mph = %f, ", speedMph_);
  printf("min = %f\n", baseTimeMin_);
}

} // namespace graph
