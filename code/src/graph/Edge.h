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

#ifndef EDGE_H
#define EDGE_H

#include "../CommonUtilities.h"
#include "../db/CatTranDatabase.h"
#include "../db/PathDatabase.h"
#include "../db/TrafficDatabase.h"
#include "Node.h"

namespace graph
{

class Node;

enum class EdgeStatus : unsigned int
{
  EDGE_OK,
  EDGE_INVALID_COST_TYPE,
  EDGE_NOT_IN_TRAFFIC_DB,
  EDGE_NOT_IN_CATTRAN_DB,
  UNKNOWN
};

class Edge
{
public:
  Edge();
  Edge(Node *src, Node *dest, TransportMode mode, PathDatabase &pdb, TrafficDatabase &tdb, CatTranDatabase &cdb);
  ~Edge();

  Edge(const Edge &copy);
  Edge &operator=(const Edge &rhs);

  Node *src();
  Node *dest();
  double cost(CostType costType, unsigned int startHr, double startMin, unsigned int &endHr, double &endMin, EdgeStatus &status);
  void print();

private:
  double distCost(EdgeStatus &status);
  double noTrafficCost(EdgeStatus &status);
  double trafficCost(unsigned int hr, double min, EdgeStatus &status);

  double trafficCoefficient(unsigned int hr, unsigned int min, EdgeStatus &status);

  Node *src_;
  Node *dest_;
  TransportMode mode_;
  double distMile_;
  double speedMph_;
  double trafficEdgeCoeff_;
  double trafficModeCoeff_;
  double baseTimeMin_;

  PathDatabase &path_;
  TrafficDatabase &traffic_;
  CatTranDatabase &cattran_;
};

} // namespace graph

#endif /* EDGE_H */
