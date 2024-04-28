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

#ifndef TRAFFIC_DATABASE_H
#define TRAFFIC_DATABASE_H

#include <map>

enum class TrafficDatabaseStatus : unsigned int
{
  TRAFFIC_OK,
  TRAFFIC_FILE_OPEN_FAILED,
  TRAFFIC_BAD_FORMAT,
  UNKNOWN
};

class TrafficDatabase
{
public:
  struct TrafficDatabaseType
  {
    unsigned int hr;
    unsigned int min;
    double volume;
  };

  TrafficDatabase();
  ~TrafficDatabase();

  TrafficDatabaseStatus read(const char *filename);
  void print();

  std::map<std::pair<unsigned int, unsigned int>, struct TrafficDatabaseType> db_;

private:
  static constexpr double CONVERSION_FACTOR = 1.0;
};

#endif /* TRAFFIC_DATABASE_H */
