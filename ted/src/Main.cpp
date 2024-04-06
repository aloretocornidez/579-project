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

#include "EdgeDatabase.h"
#include "NodeDatabase.h"

int main()
{
    EdgeDatabase edb;
    edb.read("../../data/edge.csv");
    edb.print();

    //NodeDatabase ndb;
    //ndb.read("../../data/node.csv");
    //ndb.print();

    //NodeDatabaseStatus status;
    //printf("\n%f\n", ndb.lineOfSight(6,52,status));

    return 0;
}