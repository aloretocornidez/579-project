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

#include <fstream>
#include <iomanip>
#include <iostream>
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

      std::stringstream ss(line);

      if (!(ss >> t.hr >> comma)) retVal = TrafficDatabaseStatus::TRAFFIC_BAD_FORMAT;
      if (!(ss >> t.min >> comma)) retVal = TrafficDatabaseStatus::TRAFFIC_BAD_FORMAT;
      if (!(ss >> t.volume)) retVal = TrafficDatabaseStatus::TRAFFIC_BAD_FORMAT;

      if (TrafficDatabaseStatus::TRAFFIC_BAD_FORMAT == retVal)
      {
        std::cout << "ERROR: (TrafficDatabase::read) Bad format!" << std::endl;
      }

      db_[std::make_pair(t.hr, t.min)] = t;
    }

    if (TrafficDatabaseStatus::UNKNOWN == retVal)
    {
      retVal = TrafficDatabaseStatus::TRAFFIC_OK;
    }
  }
  else
  {
    std::cout << "ERROR: (TrafficDatabase::read) fstream::open failed!" << std::endl;
    return TrafficDatabaseStatus::TRAFFIC_FILE_OPEN_FAILED;
  }

  f.close();

  return retVal;
}

void TrafficDatabase::print()
{
  for (std::map<std::pair<unsigned int, unsigned int>, struct TrafficDatabaseType>::iterator it(db_.begin()); it != db_.end(); ++it)
  {
    std::cout << std::setprecision(2) << (it->second).hr << ":" << std::setw(2) << std::setfill('0') << (it->second).min << " -  traffic coeff: " << std::setprecision(2) << (it->second).volume << std::endl;
  }
}
