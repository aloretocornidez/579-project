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

#include "PathDatabase.h"

PathDatabase::PathDatabase() {}

PathDatabase::~PathDatabase()
{
  if (true == db_.empty())
  {
    db_.clear();
  }
}

PathDatabaseStatus PathDatabase::read(const char *filename)
{
  PathDatabaseStatus retVal(PathDatabaseStatus::UNKNOWN);

  std::fstream f;
  f.open(filename, std::fstream::in);
  if (true == f.good())
  {
    struct PathDatabaseType p = {0};
    unsigned int dist_ft(0);
    unsigned int straight_ft(0);
    unsigned int mode(0);
    char comma(',');

    std::string line;
    while (std::getline(f, line))
    {
      // printf("%s\n", line.c_str());

      std::stringstream ss(line);

      if (!(ss >> p.src >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> p.dest >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> dist_ft >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> straight_ft >> comma)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;
      if (!(ss >> mode)) retVal = PathDatabaseStatus::PATH_BAD_FORMAT;

      if (PathDatabaseStatus::PATH_BAD_FORMAT == retVal)
      {
        printf("ERROR: (PathDatabase::read) Bad format!\n");
      }

      p.dist_mile = dist_ft / FT_IN_MILE;
      p.straight_mile = straight_ft / FT_IN_MILE;

      switch (mode)
      {
      case 1:
        p.mode = PathDatabaseMode::WALK;
        break;
      case 2:
        p.mode = PathDatabaseMode::WALK_BIKE;
        break;
      case 3:
        p.mode = PathDatabaseMode::WALK_BIKE_CATTRAN;
        break;
      default:
        p.mode = PathDatabaseMode::UNKNOWN;
        retVal = PathDatabaseStatus::PATH_INVALID_MODE;
        printf("ERROR: (PathDatabase::read) Bad mode!\n");
      }

      db_[std::make_pair(p.src, p.dest)] = p;

      //// Adding reverse path
      // unsigned int temp(p.src);
      // p.src = p.dest;
      // p.dest = temp;
      // rdb_[std::make_pair(p.src, p.dest)] = p;
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
    printf("%2u -> %2u, dist = %f, straight = %f, mode = ", (it->second).src, (it->second).dest, (it->second).dist_mile, (it->second).straight_mile);

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

  // for (std::map<std::pair<unsigned int, unsigned int>, struct PathDatabaseType>::iterator it(rdb_.begin());
  //     it != rdb_.end(); ++it)
  //{
  //     printf("%2u -> %2u, dist = %f, straight = %f, mode = ", (it->second).src, (it->second).dest, (it->second).dist_mile, (it->second).straight_mile);

  //    switch ((it->second).mode)
  //    {
  //    case PathDatabaseMode::UNAVAILABLE:
  //        printf("UNAVAILABLE\n");
  //        break;
  //    case PathDatabaseMode::WALK:
  //        printf("WALK\n");
  //        break;
  //    case PathDatabaseMode::WALK_BIKE:
  //        printf("WALK_BIKE\n");
  //        break;
  //    case PathDatabaseMode::WALK_BIKE_CATTRAN:
  //        printf("WALK_BIKE_CATTRAN\n");
  //        break;
  //    case PathDatabaseMode::UNKNOWN:
  //    default:
  //        printf("UNKNOWN\n");
  //    }
  //}
}
