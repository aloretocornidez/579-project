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

#include "./db/PathDatabase.h"
#include "./db/LocationDatabase.h"
#include "AStar.h"

int main()
{
  PathDatabase edb;
  edb.read("../data/paths.csv");
  // edb.print();

  LocationDatabase ndb;
  ndb.read("../data/locations.csv");
  // ndb.print();

  AStar a(edb, ndb);
  a.solve(6, 52);

  return 0;
}
