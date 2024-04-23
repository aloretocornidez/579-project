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

#ifndef ASTAR_EDGE_H
#define ASTAR_EDGE_H

#include <map>

#include "TrafficDatabase.h"

enum class AStarEdgeStatus : unsigned int
{
    EDGE_OK,
    EDGE_INVALID_MODE,
    EDGE_BAD_SPEED,
    UNKNOWN
};

class AStarEdge
{
public:
    enum class AStarEdgeCostType : unsigned int
    {
        DISTANCE,
        TIME_NO_TRAFFIC,
        TIME_WITH_TRAFFIC,
        UNKNOWN
    };
    
    enum class AStarEdgeMode : unsigned int
    {
        WALK,
        BIKE,
        CATTRAN,
        UNKNOWN
    };
    
    struct AStarEdgeType
    {
        unsigned int src;
        unsigned int dest;
        AStarEdgeMode mode;
        double dist;
    };
    
    AStarEdge(TrafficDatabase & tdb);
    ~AStarEdge();
    
    double getCost(AStarEdgeType & edge,
                   unsigned int hr, unsigned int min,
                   AStarEdgeCostType costType,
                   AStarEdgeStatus & status);
    void print();
    
    std::map<std::tuple<unsigned int, unsigned int, AStarEdgeMode>, struct AStarEdgeType> db_;
    
private:
    static constexpr double WALK_KPH = 5.0;      // km/h, ~3.11 mph
    static constexpr double BIKE_KPH = 16.0;     // km/h, ~9.94 mph
    static constexpr double CATTRAN_KPH = 12.0;  // km/h, ~6.85 mph
    static constexpr double KM_TO_MILE = 0.621371;
    
    TrafficDatabase & traffic_;
    
};

#endif /* ASTAR_EDGE_H */
