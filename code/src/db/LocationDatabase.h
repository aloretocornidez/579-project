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
  static constexpr unsigned int MAX_NEIGHBOR = 6u;

  struct LocationDatabaseType
  {
    unsigned int id;
    double latitude;
    double longitude;
    bool isDest;
    bool isCatTranStop;
    bool isBikeDepot;
    bool isIntersection;
    unsigned int numNeighbors;
    unsigned int neighbor[MAX_NEIGHBOR];
  };

  LocationDatabase();
  ~LocationDatabase();

  LocationDatabaseStatus read(const char *filename);
  double lineOfSight(unsigned int nId1, unsigned int nId2, LocationDatabaseStatus &status);

  void print();

  std::map<unsigned int, struct LocationDatabaseType> db_;

private:
  static constexpr double PI = 3.14159265359;
  static constexpr double EARTH_R = 6372818.0; // Radius of Earth for Tucson (Latitude = 32.235,
                                               //     2389 ft above Sea Level)
  static constexpr double METER_TO_FOOT = 3.2808399999999888763;
  static constexpr double METER_TO_MILE = 0.00062137121212121;
};

#endif /* LOCATION_DATABASE_H */
