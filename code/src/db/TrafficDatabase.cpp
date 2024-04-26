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
#include <fstream>
#include <sstream>
#include <string>

#include "TrafficDatabase.h"

TrafficDatabase::TrafficDatabase() {}

TrafficDatabase::~TrafficDatabase()
{
  if (true == db_.empty())
  {
    db_.clear();
  }
}

TrafficDatabaseStatus TrafficDatabase::read(const char *filename)
{
  TrafficDatabaseStatus retVal(TrafficDatabaseStatus::UNKNOWN);

  std::fstream f;
  f.open(filename, std::fstream::in);
  if (true == f.good())
  {
    struct TrafficDatabaseType t = {0};
    double trafficAmount(0.0);
    char comma(',');

    std::string line;
    while (std::getline(f, line))
    {
      // printf("%s\n", line.c_str());

      std::stringstream ss(line);

      if (!(ss >> t.hr >> comma)) retVal = TrafficDatabaseStatus::TRAFFIC_BAD_FORMAT;
      if (!(ss >> t.min >> comma)) retVal = TrafficDatabaseStatus::TRAFFIC_BAD_FORMAT;
      if (!(ss >> trafficAmount)) retVal = TrafficDatabaseStatus::TRAFFIC_BAD_FORMAT;

      if (TrafficDatabaseStatus::TRAFFIC_BAD_FORMAT == retVal)
      {
        printf("ERROR: (TrafficDatabase::read) Bad format!\n");
      }

      t.coefficient = (1 - trafficAmount) * CONVERSION_FACTOR;

      db_[std::make_pair(t.hr, t.min)] = t;
    }

    if (TrafficDatabaseStatus::UNKNOWN == retVal)
    {
      retVal = TrafficDatabaseStatus::TRAFFIC_OK;
    }
  }
  else
  {
    printf("ERROR: (TrafficDatabase::read) fstream::open failed!\n");
    return TrafficDatabaseStatus::TRAFFIC_FILE_OPEN_FAILED;
  }

  f.close();

  return retVal;
}

void TrafficDatabase::print()
{
  for (std::map<std::pair<unsigned int, unsigned int>, struct TrafficDatabaseType>::iterator it(db_.begin()); it != db_.end(); ++it)
  {
    printf("%02u:%02u -  traffic coeff: %f\n", (it->second).hr, (it->second).min, (it->second).coefficient);
  }
}
