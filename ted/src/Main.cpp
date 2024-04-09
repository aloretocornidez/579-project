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

#include "AStar.h"
#include "EdgeDatabase.h"
#include "NodeDatabase.h"

int main()
{
    EdgeDatabase edb;
    edb.read("../../data/edge.csv");
    //edb.print();

    NodeDatabase ndb;
    ndb.read("../../data/node.csv");
    //ndb.print();

    AStar a(edb, ndb);
    a.solve(6, 52);

    return 0;
}