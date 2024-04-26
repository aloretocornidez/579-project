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

#include "./db/CatTranDatabase.h"
#include "./db/LocationDatabase.h"
#include "./db/PathDatabase.h"
#include "./db/TrafficDatabase.h"
#include "./graph/Graph.h"
#include "AStar.h"

int main()
{
  PathDatabase pdb;
  pdb.read("../data/edge.csv");
  // pdb.print();

  LocationDatabase ldb;
  ldb.read("../data/node.csv");
  // ldb.print();

  TrafficDatabase tdb;
  tdb.read("../data/traffic.csv");
  // tdb.print();

  CatTranDatabase cdb;
  cdb.read("../data/cattran.csv");
  // cdb.print();

  graph::Graph g(ldb, pdb, tdb, cdb);
  g.build();
  // g.print();

  astar::AStar a(g);
  a.solve(52, 2, CostType::TIME_WITH_TRAFFIC, 7, 2.0);

  return 0;
}
