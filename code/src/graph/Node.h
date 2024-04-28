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

#ifndef NODE_H
#define NODE_H

#include "../CommonUtilities.h"
#include "../db/LocationDatabase.h"
#include "Edge.h"

namespace graph
{

class Edge;

enum class NodeStatus : unsigned int
{
  NODE_OK,
  NODE_CHILDREN_FULL,
  NODE_CHILD_NOT_NULL,
  NODE_INCOMING_FULL,
  NODE_INCOMING_NOT_NULL,
  NODE_OUTGOING_FULL,
  NODE_OUTGOING_NOT_NULL,
  NODE_LOS_BAD_ID,
  UNKNOWN
};

class Node
{
public:
  Node();
  Node(unsigned int id, TransportMode mode, LocationDatabase &ldb);
  ~Node();

  Node(const Node &copy);
  Node &operator=(const Node &rhs);

  void setIsDest(bool isDest);
  void setIsCatTranStop(bool isCatTranStop);
  void setIsBikeDepot(bool isBikeDepot);

  NodeStatus addChild(Node *cNode);
  NodeStatus addIncoming(Edge *inEdge);
  NodeStatus addOutgoing(Edge *outEdge);

  unsigned int id();
  TransportMode mode();
  unsigned int numChildren();

  Node *child(unsigned int index);
  Edge *outgoing(unsigned int index);

  void print();

private:
  static constexpr unsigned int MAX_EDGES = 15u;

  unsigned int id_;
  TransportMode type_;

  bool isDest_;
  bool isCatTranStop_;
  bool isBikeDepot_;

  Node *children_[MAX_EDGES];

  Edge *incoming_[MAX_EDGES];
  Edge *outgoing_[MAX_EDGES];

  unsigned int childCount_;
  unsigned int inCount_;
  unsigned int outCount_;

  LocationDatabase &loc_;
};

} // namespace graph

#endif /* NODE_H */
