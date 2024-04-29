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

#include "AStarEdge.h"

AStarEdge::AStarEdge(TrafficDatabase & tdb)
    : traffic_(tdb)
{
    
}

AStarEdge::~AStarEdge()
{
    
}

double AStarEdge::getCost(AStarEdgeType & edge,
                          unsigned int hr, unsigned int min,
                          AStarEdgeCostType costType,
                          AStarEdgeStatus & status)
{
    status = AStarEdgeStatus::UNKNOWN;
    
    if (AStarEdgeCostType::DISTANCE == costType)
    {
        status = AStarEdgeStatus::EDGE_OK;
        return edge.dist;
    }
    else
    {
        double speed(0.0);
        switch (edge.mode)
        {
            case AStarEdgeMode::WALK:
                speed = WALK_KPH * KM_TO_MILE;
                break;
            case AStarEdgeMode::BIKE:
                speed = BIKE_KPH * KM_TO_MILE;
                break;
            case AStarEdgeMode::CATTRAN:
                speed = CATTRAN_KPH * KM_TO_MILE;
                break;
            case AStarEdgeMode::UNKNOWN:
            default:
                speed = 0.0;
                status = AStarEdgeStatus::EDGE_INVALID_MODE;
                printf("ERROR: (AStarEdge::getCost) Bad mode!\n");
        }
        
        if (AStarEdgeCostType::TIME_WITH_TRAFFIC == costType)
        {
            struct TrafficDatabase::TrafficDatabaseType t = traffic_.db_.at(std::make_pair(hr, min));
            speed *= t.coefficient;
        }
        
        double time(std::numeric_limits<double>::max());
        if (0.0 < speed)
        {
            status = AStarEdgeStatus::EDGE_OK;
            time = edge.dist / speed;
        }
        else
        {
            status = AStarEdgeStatus::EDGE_BAD_SPEED;
            printf("ERROR: (AStarEdge::getCost) Speed not positive!\n");
        }

        return time;
    }
}

void AStarEdge::print()
{
    std::map<std::tuple<unsigned int, unsigned int, AStarEdgeMode>, struct AStarEdgeType>::iterator it;
    for (it = db_.begin(); it != db_.end(); ++it)
    {
        printf("%2u -> %2u, dist = %f, mode = ",
               (it->second).src, (it->second).dest, (it->second).dist);

        switch((it->second).mode)
        {
        case AStarEdgeMode::WALK:
            printf("WALK\n");
            break;
        case AStarEdgeMode::BIKE:
            printf("BIKE\n");
            break;
        case AStarEdgeMode::CATTRAN:
            printf("CATTRAN\n");
            break;
        case AStarEdgeMode::UNKNOWN:
        default:
            printf("UNKNOWN\n");
        }
    }
}
