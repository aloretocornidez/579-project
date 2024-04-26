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

#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include "LocationDatabase.h"

LocationDatabase::LocationDatabase() {}

LocationDatabase::~LocationDatabase()
{
  if (true == db_.empty())
  {
    db_.clear();
  }
}

LocationDatabaseStatus LocationDatabase::read(const char *filename)
{
  LocationDatabaseStatus retVal(LocationDatabaseStatus::UNKNOWN);

  std::fstream f;
  f.open(filename, std::fstream::in);
  if (true == f.good())
  {
    struct LocationDatabaseType l = {0};
    unsigned int dest(0);
    unsigned int ctStop(0);
    unsigned int bDepot(0);
    unsigned int inter(0);
    unsigned int neighbor(0);
    char comma(',');

    std::string line;
    while (std::getline(f, line))
    {
      // printf("%s\n", line.c_str());

      std::stringstream ss(line);

      if (!(ss >> l.id >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> l.latitude >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> l.longitude >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> dest >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> ctStop >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> bDepot >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> inter >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;

      if (LocationDatabaseStatus::LOCATION_BAD_FORMAT == retVal)
      {
        printf("ERROR: (LocationDatabase::read) Bad format!\n");
      }

      l.isDest = (dest == 1);
      l.isCatTranStop = (ctStop == 1);
      l.isBikeDepot = (bDepot == 1);
      l.isIntersection = (inter == 1);

      l.numNeighbors = 0;
      memset(l.neighbor, 0, sizeof(l.neighbor));
      while (ss >> neighbor)
      {
        l.neighbor[l.numNeighbors] = neighbor;
        ++l.numNeighbors;
        ss >> comma;
      }

      db_[l.id] = l;
    }

    if (LocationDatabaseStatus::UNKNOWN == retVal)
    {
      retVal = LocationDatabaseStatus::LOCATION_OK;
    }
  }
  else
  {
    printf("ERROR: (LocationDatabase::read) fstream::open failed!\n");
    return LocationDatabaseStatus::LOCATION_FILE_OPEN_FAILED;
  }

  f.close();

  return retVal;
}

double LocationDatabase::lineOfSight(unsigned int nId1, unsigned int nId2, LocationDatabaseStatus &status)
{
  LocationDatabaseStatus retVal(LocationDatabaseStatus::UNKNOWN);
  double los(0.0);

  if ((db_.find(nId1) != db_.end()) && (db_.find(nId2) != db_.end()))
  {
    double lat1 = db_[nId1].latitude * PI / 180.0;
    double long1 = db_[nId1].longitude * PI / 180.0;
    double lat2 = db_[nId2].latitude * PI / 180.0;
    double long2 = db_[nId2].longitude * PI / 180.0;

    los = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(long2 - long1)) * EARTH_R * METER_TO_MILE;

    retVal = LocationDatabaseStatus::LOCATION_OK;
  }
  else
  {
    printf("ERROR: (LocationDatabase::lineOfSight) Location ID not found!\n");
    retVal = LocationDatabaseStatus::LOCATION_INVALID_ID;
  }

  status = retVal;
  return los;
}

void LocationDatabase::print()
{
  for (std::map<unsigned int, struct LocationDatabaseType>::iterator it(db_.begin()); it != db_.end(); ++it)
  {
    printf("%2u: (%f,%f) ", (it->second).id, (it->second).latitude, (it->second).longitude);

    if (true == (it->second).isDest)
    {
      printf("Dest ");
    }

    if (true == (it->second).isCatTranStop)
    {
      printf("CalTranStop ");
    }

    if (true == (it->second).isBikeDepot)
    {
      printf("BikeDepot ");
    }

    if (true == (it->second).isIntersection)
    {
      printf("Intersecton ");
    }

    printf("- (%d) ", (it->second).numNeighbors);

    for (unsigned int i(0); i < (it->second).numNeighbors; ++i)
    {
      if (0 == i)
      {
        printf("%u", (it->second).neighbor[i]);
      }
      else
      {
        printf(",%u", (it->second).neighbor[i]);
      }
    }
    printf("\n");
  }
}
