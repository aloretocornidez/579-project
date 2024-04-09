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

#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "NodeDatabase.h"

const double NodeDatabase::PI(3.14159265359);
const double NodeDatabase::EARTH_R(6372818.0); // Radius of Earth for Tucson (Latitude = 32.235, 2389ft above Sea Level)
const double NodeDatabase::METER_TO_FOOT(3.2808399999999888763);
const double NodeDatabase::METER_TO_MILE(0.00062137121212121);

NodeDatabase::NodeDatabase()
{
}

NodeDatabase::~NodeDatabase()
{
}

NodeDatabaseStatus NodeDatabase::read(const char * filename)
{
    NodeDatabaseStatus retVal(NodeDatabaseStatus::UNKNOWN);

    std::fstream f;
    f.open(filename, std::fstream::in);
    if (true == f.good())
    {
        struct NodeDatabaseType n = { 0, false, false, false, false, 0, {0} };
        unsigned int dest(0);
        unsigned int ctStop(0);
        unsigned int bDepot(0);
        unsigned int inter(0);
        unsigned int neighbor(0);
        char comma;

        std::string line;
        while (std::getline(f, line))
        {
            //printf("%s\n", line.c_str());

            std::stringstream ss(line);

            if (!(ss >> n.id >> comma))        retVal = NodeDatabaseStatus::NODE_BAD_FORMAT;
            if (!(ss >> n.latitude >> comma))  retVal = NodeDatabaseStatus::NODE_BAD_FORMAT;
            if (!(ss >> n.longitude >> comma)) retVal = NodeDatabaseStatus::NODE_BAD_FORMAT;
            if (!(ss >> dest >> comma))        retVal = NodeDatabaseStatus::NODE_BAD_FORMAT;
            if (!(ss >> ctStop >> comma))      retVal = NodeDatabaseStatus::NODE_BAD_FORMAT;
            if (!(ss >> bDepot >> comma))      retVal = NodeDatabaseStatus::NODE_BAD_FORMAT;
            if (!(ss >> inter >> comma))       retVal = NodeDatabaseStatus::NODE_BAD_FORMAT;

            if (NodeDatabaseStatus::NODE_BAD_FORMAT == retVal)
            {
                printf("ERROR: (NodeDatabase::read) Bad format!\n");
            }

            n.isDest         = (dest == 1);
            n.isCatTranStop  = (ctStop == 1);
            n.isBikeDepot    = (bDepot == 1);
            n.isIntersection = (inter == 1);

            n.numNeighbors = 0;
            memset(n.neighbor, 0, sizeof(n.neighbor));
            while (ss >> neighbor)
            {
                n.neighbor[n.numNeighbors] = neighbor;
                ++n.numNeighbors;
                ss >> comma;
            }

            db_[n.id] = n;
        }

        if (NodeDatabaseStatus::UNKNOWN == retVal)
        {
            retVal = NodeDatabaseStatus::NODE_OK;
        }
    }
    else
    {
        printf("ERROR: (NodeDatabase::read) fstream::open failed!\n");
        return NodeDatabaseStatus::NODE_FILE_OPEN_FAILED;
    }

    f.close();

    return retVal;
}

double NodeDatabase::lineOfSight(unsigned int nId1, unsigned int nId2, NodeDatabaseStatus & status)
{
    NodeDatabaseStatus retVal(NodeDatabaseStatus::UNKNOWN);
    double los(0.0);

    if ((db_.find(nId1) != db_.end()) && (db_.find(nId2) != db_.end()))
    {
        double lat1  = db_[nId1].latitude * PI / 180.0;
        double long1 = db_[nId1].longitude * PI / 180.0;
        double lat2  = db_[nId2].latitude * PI / 180.0;
        double long2 = db_[nId2].longitude * PI / 180.0;

        los = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(long2 - long1)) * EARTH_R * METER_TO_MILE;

        retVal = NodeDatabaseStatus::NODE_OK;
    }
    else
    {
        printf("ERROR: (NodeDatabase::lineOfSight) Node ID not found!\n");
        retVal = NodeDatabaseStatus::NODE_INVALID_ID;
    }
    
    status = retVal;
    return los;
}

void NodeDatabase::print()
{
    for (std::map<unsigned int, struct NodeDatabaseType>::iterator it(db_.begin()); 
         it != db_.end(); ++it)
    {
        printf("%2u: (%f,%f) ", (it->second).id, (it->second).latitude, (it->second).longitude);

        if (true == (it->second).isDest)
        {
            printf("Dest ");
        }

        if (true == (it->second).isCatTranStop)
        {
            printf("CalTranStop ");
        }

        if (true == (it->second).isBikeDepot)
        {
            printf("BikeDepot ");
        }

        if (true == (it->second).isIntersection)
        {
            printf("Intersecton ");
        }

        printf("- (%d) ", (it->second).numNeighbors);

        for (unsigned int i(0); i < (it->second).numNeighbors; ++i)
        {
            if (0 == i)
            {
                printf("%u", (it->second).neighbor[i]);
            }
            else
            {
                printf(",%u", (it->second).neighbor[i]);
            }
        }
        printf("\n");
    }
}