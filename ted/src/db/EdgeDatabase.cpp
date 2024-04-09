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
#include <fstream>
#include <sstream>
#include <string>

#include "EdgeDatabase.h"

const double EdgeDatabase::FT_IN_MILE(5280.0);

EdgeDatabase::EdgeDatabase()
{
}

EdgeDatabase::~EdgeDatabase()
{
}

EdgeDatabaseStatus EdgeDatabase::read(const char * filename)
{
    EdgeDatabaseStatus retVal(EdgeDatabaseStatus::UNKNOWN);

    std::fstream f;
    f.open(filename, std::fstream::in);
    if (true == f.good())
    {
        struct EdgeDatabaseType e = {0, 0, 0.0, 0.0, EdgeDatabaseMode::UNKNOWN};
        unsigned int dist_ft(0);
        unsigned int straight_ft(0);
        unsigned int mode(0);
        char comma;

        std::string line;
        while (std::getline(f, line))
        {
            //printf("%s\n", line.c_str());

            std::stringstream ss(line);

            if (!(ss >> e.src >> comma))       retVal = EdgeDatabaseStatus::EDGE_BAD_FORMAT;
            if (!(ss >> e.dest >> comma))      retVal = EdgeDatabaseStatus::EDGE_BAD_FORMAT;
            if (!(ss >> dist_ft >> comma))     retVal = EdgeDatabaseStatus::EDGE_BAD_FORMAT;
            if (!(ss >> straight_ft >> comma)) retVal = EdgeDatabaseStatus::EDGE_BAD_FORMAT;
            if (!(ss >> mode))                 retVal = EdgeDatabaseStatus::EDGE_BAD_FORMAT;

            if (EdgeDatabaseStatus::EDGE_BAD_FORMAT == retVal)
            {
                printf("ERROR: (EdgeDatabase::read) Bad format!\n");
            }

            e.dist_mile     = dist_ft / FT_IN_MILE;
            e.straight_mile = straight_ft / FT_IN_MILE;

            switch (mode)
            {
            case 1:
                e.mode = EdgeDatabaseMode::WALK;
                break;
            case 2:
                e.mode = EdgeDatabaseMode::WALK_BIKE;
                break;
            case 3:
                e.mode = EdgeDatabaseMode::WALK_BIKE_CATTRAN;
                break;
            default:
                e.mode = EdgeDatabaseMode::UNKNOWN;
                retVal = EdgeDatabaseStatus::EDGE_INVALID_MODE;
                printf("ERROR: (EdgeDatabase::read) Bad mode!\n");
            }

            db_[std::make_pair(e.src, e.dest)] = e;
        }

        if (EdgeDatabaseStatus::UNKNOWN == retVal)
        {
            retVal = EdgeDatabaseStatus::EDGE_OK;
        }
    }
    else
    {
        printf("ERROR: (EdgeDatabase::read) fstream::open failed!\n");
        return EdgeDatabaseStatus::EDGE_FILE_OPEN_FAILED;
    }

    f.close();

    return retVal;
}

void EdgeDatabase::print()
{
    for (std::map<std::pair<unsigned int, unsigned int>, struct EdgeDatabaseType>::iterator it(db_.begin()); 
         it != db_.end(); ++it)
    {
        printf("%2u -> %2u, dist = %f, straight = %f, mode = ", (it->second).src, (it->second).dest, (it->second).dist_mile, (it->second).straight_mile);

        switch((it->second).mode)
        {
        case EdgeDatabaseMode::UNAVAILABLE:
            printf("UNAVAILABLE\n");
            break;
        case EdgeDatabaseMode::WALK:
            printf("WALK\n");
            break;
        case EdgeDatabaseMode::WALK_BIKE:
            printf("WALK_BIKE\n");
            break;
        case EdgeDatabaseMode::WALK_BIKE_CATTRAN:
            printf("WALK_BIKE_CATTRAN\n");
            break;
        case EdgeDatabaseMode::UNKNOWN:
        default:
            printf("UNKNOWN\n");
        }
    }
}
