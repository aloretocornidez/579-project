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

#include <iostream>

#include "AStar.h"
#include "db/CatTranDatabase.h"
#include "db/LocationDatabase.h"
#include "db/PathDatabase.h"
#include "db/TrafficDatabase.h"
#include "graph/Graph.h"
#include "parseArgs.h"

int main(int argc, char *argv[])
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

  Parser parser;

  switch (parser.parseArguments(argc, argv))
  {
  case Parser::parseArgsRetVal::ERROR_ARGS:
    return 0;
    break;

  case Parser::parseArgsRetVal::HELP_ARGS:
    parser.printUsage();
    break;
  case Parser::parseArgsRetVal::USER_ARGS:
  case Parser::parseArgsRetVal::DEFAULT_ARGS:
    parser.PrintParams();
    break;
  }

  Parser::Parameters params = parser.getParams();

  a.solve(params.startId, params.goalId, params.costType, params.hr, params.min);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 50.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 51.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 52.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 53.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 54.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 55.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 56.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 57.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 58.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 11, 59.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 00.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 01.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 02.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 03.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 04.0);
  // a.solve(52, 41, CostType::TIME_WITH_TRAFFIC, 12, 05.0);

  return 0;
}
