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

#ifndef NODE_DATABASE_H
#define NODE_DATABASE_H

#include <map>

enum class NodeDatabaseStatus : unsigned int
{
    NODE_OK,
    NODE_FILE_OPEN_FAILED,
    NODE_BAD_FORMAT,
    NODE_INVALID_ID,
    UNKNOWN
};

class NodeDatabase
{
public:
    struct NodeDatabaseType
    {
        unsigned int id;
        double latitude;
        double longitude;
        bool isDest;
        bool isCatTranStop;
        bool isBikeDepot;
        bool isIntersection;
        unsigned int numNeighbors;
        unsigned int neighbor[10];
    };

    NodeDatabase();
    ~NodeDatabase();

    NodeDatabaseStatus read(const char * filename);
    double lineOfSight(unsigned int nId1, unsigned int nId2, NodeDatabaseStatus & status);

    void print();

    std::map<unsigned int, struct NodeDatabaseType> db_;

private:
    static const double PI;
    static const double EARTH_R;
    static const double METER_TO_FOOT;
    static const double METER_TO_MILE;

};

#endif /* NODE_DATABASE_H */
