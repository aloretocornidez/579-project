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
#include "CatTranDatabase.h"
#include "Graph.h"
#include "LocationDatabase.h"
#include "PathDatabase.h"
#include "TrafficDatabase.h"

int main()
{
    PathDatabase pdb;
    pdb.read("../../data/edge.csv");
    //pdb.print();

    LocationDatabase ldb;
    ldb.read("../../data/node.csv");
    //ldb.print();

    TrafficDatabase tdb;
    tdb.read("../../data/traffic.csv");
    //tdb.print();

    CatTranDatabase cdb;
    cdb.read("../../data/cattran.csv");
    //cdb.print();

    graph::Graph g(ldb, pdb, tdb, cdb);
    g.build();
    //g.print();

    astar::AStar a(g);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 7, 2.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 50.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 51.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 52.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 53.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 54.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 55.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 56.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 57.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 58.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 59.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 00.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 01.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 02.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 03.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 04.0);
    a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 05.0);

    return 0;
}
