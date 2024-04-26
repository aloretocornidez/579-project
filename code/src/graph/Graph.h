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

#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <map>

#include "Edge.h"
#include "Node.h"

#include "../db/CatTranDatabase.h"
#include "../db/LocationDatabase.h"
#include "../db/PathDatabase.h"
#include "../db/TrafficDatabase.h"

namespace graph
{

enum class GraphStatus : unsigned int
{
  GRAPH_OK,
  GRAPH_BAD_PATH_MODE,
  GRAPH_NOT_IN_LOCATION_DB,
  GRAPH_NOT_IN_PATH_DB,
  GRAPH_LOS_BAD_ID,
  UNKNOWN
};

class Graph
{
public:
  static constexpr unsigned int ALL = 0;

  Graph(LocationDatabase &ldb, PathDatabase &pdb, TrafficDatabase &tdb, CatTranDatabase &cdb);
  ~Graph();

  GraphStatus build();

  double lineOfSight(unsigned int nId1, unsigned int nId2, GraphStatus &status);
  void print(unsigned int id = ALL);

  std::map<std::pair<unsigned int, TransportMode>, Node *> nodes_;
  std::map<std::tuple<unsigned int, unsigned int, TransportMode>, Edge *> edges_;

private:
  GraphStatus buildNodes();
  GraphStatus buildEdges();
  GraphStatus finishBuild();

  LocationDatabase &loc_;
  PathDatabase &path_;
  TrafficDatabase &traffic_;
  CatTranDatabase &cattran_;

  std::list<Node *> nodeList_;
  std::list<Edge *> edgeList_;
};

} // namespace graph

#endif /* NODE_H */
