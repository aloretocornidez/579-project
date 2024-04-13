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

enum class EdgeDatabaseStatus : unsigned int
{
  EDGE_OK,
  EDGE_FILE_OPEN_FAILED,
  EDGE_BAD_FORMAT,
  EDGE_INVALID_MODE,
  UNKNOWN
};

class EdgeDatabase
{
public:
  enum class EdgeDatabaseMode : unsigned int
  {
    UNAVAILABLE,
    WALK,
    WALK_BIKE,
    WALK_BIKE_CATTRAN,
    UNKNOWN
  };

  struct EdgeDatabaseType
  {
    unsigned int src;
    unsigned int dest;
    double dist_mile;
    double straight_mile;
    EdgeDatabaseMode mode;
  };

  EdgeDatabase();
  ~EdgeDatabase();

  EdgeDatabaseStatus read(const char *filename);
  void print();

  std::map<std::pair<unsigned int, unsigned int>, struct EdgeDatabaseType> db_;

private:
  static const double FT_IN_MILE;
};

#endif /* EDGE_DATABASE_H */
