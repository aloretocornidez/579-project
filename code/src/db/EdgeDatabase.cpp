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
#include <iostream>
#include <sstream>
#include <string>

#include "EdgeDatabase.h"

const double PathDatabase::FT_IN_MILE(5280.0);

PathDatabase::PathDatabase() {}

PathDatabase::~PathDatabase() {}

PathDatabaseStatus PathDatabase::read(const char *filename)
{
  PathDatabaseStatus retVal(PathDatabaseStatus::UNKNOWN);

  std::fstream f;
  f.open(filename, std::fstream::in);
  if (true == f.good())
  {
    struct PathDatabaseType e = {0, 0, 0, 0.0, 0.0, PathDatabaseMode::UNKNOWN};
    unsigned int dist_ft(0);
    unsigned int straight_ft(0);
    unsigned int mode(0);
    char comma;

    std::string line;
    while (std::getline(f, line))
    {
      // printf("%s\n", line.c_str());

      std::stringstream ss(line);

      if (!(ss >> e.path_id >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> e.src >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> e.dest >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> dist_ft >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> straight_ft >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> mode)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;

      if (PathDatabaseStatus::PATH_BAD_FORMAT == retVal)
      {
        printf("ERROR: (EdgeDatabase::read) Bad format!\n");
      }

      e.dist_mile = dist_ft / FT_IN_MILE;
      e.straight_mile = straight_ft / FT_IN_MILE;

      switch (mode)
      {
      case 1:
        e.mode = PathDatabaseMode::WALK;
        break;
      case 2:
        e.mode = PathDatabaseMode::WALK_BIKE;
        break;
      case 3:
        e.mode = PathDatabaseMode::WALK_BIKE_CATTRAN;
        break;
      default:
        e.mode = PathDatabaseMode::UNKNOWN;
        retVal = PathDatabaseStatus::PATH_INVALID_MODE;
        printf("ERROR: (EdgeDatabase::read) Bad mode!\n");
        std::cout << line << std::endl;
      }

      db_[std::make_pair(e.src, e.dest)] = e;
    }

    if (PathDatabaseStatus::UNKNOWN == retVal)
    {
      retVal = PathDatabaseStatus::PATH_OK;
    }
  }
  else
  {
    printf("ERROR: (PathDatabase::read) fstream::open failed!\n");
    return PathDatabaseStatus::PATH_FILE_OPEN_FAILED;
  }

  f.close();

  return retVal;
}

void PathDatabase::print()
{
  for (std::map<std::pair<unsigned int, unsigned int>, struct PathDatabaseType>::iterator it(db_.begin()); it != db_.end(); ++it)
  {
    printf("path_id = %2i %2u -> %2u, dist = %f, straight = %f, mode = ", (it->second).path_id, (it->second).src, (it->second).dest, (it->second).dist_mile, (it->second).straight_mile);

    switch ((it->second).mode)
    {
    case PathDatabaseMode::UNAVAILABLE:
      printf("UNAVAILABLE\n");
      break;
    case PathDatabaseMode::WALK:
      printf("WALK\n");
      break;
    case PathDatabaseMode::WALK_BIKE:
      printf("WALK_BIKE\n");
      break;
    case PathDatabaseMode::WALK_BIKE_CATTRAN:
      printf("WALK_BIKE_CATTRAN\n");
      break;
    case PathDatabaseMode::UNKNOWN:
    default:
      printf("UNKNOWN\n");
    }
  }
}
