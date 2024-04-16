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

#ifndef LOCATION_DATABASE_H
#define LOCATION_DATABASE_H

#include <map>

enum class LocationDatabaseStatus : unsigned int
{
  LOCATION_OK,
  LOCATION_FILE_OPEN_FAILED,
  LOCATION_BAD_FORMAT,
  LOCATION_INVALID_ID,
  UNKNOWN
};

class LocationDatabase
{
public:
  struct Location
  {
    unsigned int location_id;
    double latitude;
    double longitude;
    bool isDest;
    bool isCatTranStop;
    bool isBikeDepot;
    bool isIntersection;
    unsigned int num_outbound_paths;
    unsigned int outbound_paths[10];
  };

  LocationDatabase();
  ~LocationDatabase();

  LocationDatabaseStatus read(const char *filename);

  double lineOfSight(unsigned int nId1, unsigned int nId2, LocationDatabaseStatus &status);

  void print();

  std::map<unsigned int, struct Location> db_;

private:
  static const double PI;
  static const double EARTH_R;
  static const double METER_TO_FOOT;
  static const double METER_TO_MILE;
};

#endif /* LOCATION_DATABASE_H */
