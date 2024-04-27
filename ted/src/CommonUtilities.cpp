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
#include <limits>

#include "CommonUtilities.h"

const char * tModeStr(TransportMode t)
{
    switch (t)
    {
    case TransportMode::NONE:
        return "NONE";
    case TransportMode::WALK:
        return "WALK";
    case TransportMode::BIKE:
        return "BIKE";
    case TransportMode::CATTRAN:
        return "CATTRAN";
    case TransportMode::UNKNOWN:
    default:
        return "UNKNOWN";
    }
}

char tModeChar(TransportMode t)
{
    switch (t)
    {
    case TransportMode::NONE:
        return 'n';
    case TransportMode::WALK:
        return 'w';
    case TransportMode::BIKE:
        return 'b';
    case TransportMode::CATTRAN:
        return 'c';
    case TransportMode::UNKNOWN:
    default:
        return 'u';
    }
}

unsigned int toNodeCode(unsigned int id, TransportMode mode)
{
    return id * 10 + static_cast<unsigned int>(mode);
}

void fromNodeCode(unsigned int code, unsigned int & id, TransportMode & mode)
{
    mode = static_cast<TransportMode>(code % 10);
    id = code / 10;
}

double fastestMode()
{
    if ((BIKE_KPH >= WALK_KPH) && (BIKE_KPH >= CATTRAN_KPH))
    {
        return BIKE_KPH;
    }

    if ((CATTRAN_KPH >= WALK_KPH) && (CATTRAN_KPH >= BIKE_KPH))
    {
        return CATTRAN_KPH;
    }

    if ((WALK_KPH >= BIKE_KPH) && (WALK_KPH >= CATTRAN_KPH))
    {
        return WALK_KPH;
    }
}

double toTrafficEdgeCoeff(unsigned int rating)
{
    double retVal(std::numeric_limits<double>::max());

    switch (rating)
    {
    case 5:
        retVal = TRAFFIC_RATING_5;
        break;
    case 4:
        retVal = TRAFFIC_RATING_4;
        break;
    case 3:
        retVal = TRAFFIC_RATING_3;
        break;
    case 2:
        retVal = TRAFFIC_RATING_2;
        break;
    case 1:
        retVal = TRAFFIC_RATING_1;
        break;
    default:
        printf("ERROR: (CommonUtilities::toTrafficRating) Bad traffic rating!\n");
    }

    return retVal;
}

void addMin(double minToAdd,
            unsigned int startHr, double startMin,
            unsigned int & endHr, double & endMin)
{
    endMin = startMin + minToAdd;
    if (endMin >= MIN_IN_HR)
    {
        unsigned int hrDelta = static_cast<unsigned int>(endMin / MIN_IN_HR);

        endHr = startHr + hrDelta;
        endMin = endMin - (hrDelta * MIN_IN_HR);
    }
    else
    {
        endHr = startHr;
    }
}