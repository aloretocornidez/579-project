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

#ifndef COMMON_UTILITIES_H
#define COMMON_UTILITIES_H

enum class TransportMode : unsigned int
{
  NONE,
  WALK,
  BIKE,
  CATTRAN,
  UNKNOWN
};

const char *tModeStr(TransportMode t);
char tModeChar(TransportMode t);

unsigned int toNodeCode(unsigned int id, TransportMode mode);
void fromNodeCode(unsigned int code, unsigned int &id, TransportMode &mode);

enum class CostType : unsigned int
{
  DISTANCE,
  TIME_NO_TRAFFIC,
  TIME_WITH_TRAFFIC,
  UNKNOWN
};

static constexpr double WALK_KPH = 5.0;     // km/h, ~3.11 mph
static constexpr double BIKE_KPH = 16.0;    // km/h, ~9.94 mph
static constexpr double CATTRAN_KPH = 12.0; // km/h, ~6.85 mph

static constexpr double KM_TO_MILE = 0.621371;
static constexpr double HR_TO_MIN = 60.0;
static constexpr double MIN_IN_HR = 60.0;

double fastestMode();

void addMin(double minToAdd, unsigned int startHr, double startMin, unsigned int &endHr, double &endMin);

// class CommonUtilities
//{
// public:
//     CommonUtilities() {}
//     ~CommonUtilities() {}
//
// private:
//
// };

#endif /* COMMON_UTILITIES_H */
