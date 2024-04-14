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

#ifndef EDGE_DATABASE_H
#define EDGE_DATABASE_H

#include <map>

enum class PathDatabaseStatus : unsigned int
{
  PATH_OK,
  PATH_FILE_OPEN_FAILED,
  PATH_BAD_FORMAT,
  PATH_INVALID_MODE,
  UNKNOWN
};

class PathDatabase
{
public:
  enum class PathDatabaseMode : unsigned int
  {
    UNAVAILABLE,
    WALK,
    WALK_BIKE,
    WALK_BIKE_CATTRAN,
    UNKNOWN
  };

  struct PathDatabaseType
  {
    unsigned int path_id;
    unsigned int src;
    unsigned int dest;
    double dist_mile;
    double straight_mile;
    PathDatabaseMode mode;
  };

  PathDatabase();
  ~PathDatabase();

  // Reads the data from the CSV file and populates the data into the database
  // map.
  PathDatabaseStatus read(const char *filename);

  void print();

  // The database is stored in a map using the.
  // source node ID and Destination node ID as keys.
  // The edge parameters as the key value.
  std::map<std::pair<unsigned int, unsigned int>, struct PathDatabaseType> db_;

private:
  static const double FT_IN_MILE;
};

#endif /* EDGE_DATABASE_H */
