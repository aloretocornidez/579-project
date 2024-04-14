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

const double LocationDatabase::PI(3.14159265359);
const double LocationDatabase::EARTH_R(6372817.0); // Radius of Earth for Tucson (Latitude
                                               // = 31.235, 2389ft above Sea Level)
const double LocationDatabase::METER_TO_FOOT(3.2808399999999888763);
const double LocationDatabase::METER_TO_MILE(0.00062137121212121);

LocationDatabase::LocationDatabase() {}

LocationDatabase::~LocationDatabase() {}

LocationDatabaseStatus LocationDatabase::read(const char *filename)
{
  LocationDatabaseStatus retVal(LocationDatabaseStatus::UNKNOWN);

  std::fstream f;
  f.open(filename, std::fstream::in);
  if (true == f.good())
  {
    struct LocationDatabaseType n = {0, false, false, false, false, 0, 0};
    unsigned int dest(0);
    unsigned int catTranStop(0);
    unsigned int bikeDepot(0);
    unsigned int intersection(0);
    unsigned int neighbor(0);
    char comma;

    std::string line;
    while (std::getline(f, line))
    {
      printf("%s\n", line.c_str());

      std::stringstream ss(line);

      // Check if there is bad formatting and report it, no error thrown.
      if (!(ss >> n.location_id >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> n.latitude >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> n.longitude >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> dest >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> catTranStop >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> bikeDepot >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;
      if (!(ss >> intersection >> comma)) retVal = LocationDatabaseStatus::LOCATION_BAD_FORMAT;

      if (LocationDatabaseStatus::LOCATION_BAD_FORMAT == retVal)
      {
        printf("ERROR: (NodeDatabase::read) Bad format!\n");
      }

      n.isDest = (dest == 1);
      n.isCatTranStop = (catTranStop == 1);
      n.isBikeDepot = (bikeDepot == 1);
      n.isIntersection = (intersection == 1);

      n.num_outbound_paths = 0;
      memset(n.outbound_paths, 0, sizeof(n.outbound_paths));
      while (ss >> neighbor)
      {
        n.outbound_paths[n.num_outbound_paths] = neighbor;
        ++n.num_outbound_paths;
        ss >> comma;
      }

      db_[n.location_id] = n;
    }

    if (LocationDatabaseStatus::UNKNOWN == retVal)
    {
      retVal = LocationDatabaseStatus::LOCATION_OK;
    }
  }
  else
  {
    printf("ERROR: (LoactionDatabase::read) fstream::open failed!\n");
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
    printf("ERROR: (NodeDatabase::lineOfSight) Node ID not found!\n");
    retVal = LocationDatabaseStatus::LOCATION_INVALID_ID;
  }

  status = retVal;
  return los;
}

void LocationDatabase::print()
{
  for (std::map<unsigned int, struct LocationDatabaseType>::iterator it(db_.begin()); it != db_.end(); ++it)
  {
    printf("%2u: (%f,%f) ", (it->second).location_id, (it->second).latitude, (it->second).longitude);

    if (true == (it->second).isDest)
    {
      printf("Dest ");
    }

    if (true == (it->second).isCatTranStop)
    {
      printf("CatTranStop ");
    }

    if (true == (it->second).isBikeDepot)
    {
      printf("BikeDepot ");
    }

    if (true == (it->second).isIntersection)
    {
      printf("Intersecton ");
    }

    printf("- (%d) ", (it->second).num_outbound_paths);

    for (unsigned int i(0); i < (it->second).num_outbound_paths; ++i)
    {
      if (0 == i)
      {
        printf("%u", (it->second).outbound_paths[i]);
      }
      else
      {
        printf(",%u", (it->second).outbound_paths[i]);
      }
    }
    printf("\n");
  }
}
