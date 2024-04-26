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
#include <cstring>

#include "Node.h"

namespace graph
{

LocationDatabase dummylocation;

Node::Node()
    : id_(0)
    , type_(TransportMode::UNKNOWN)
    , isDest_(false)
    , isCatTranStop_(false)
    , isBikeDepot_(false)
    , children_{0}
    , incoming_{0}
    , outgoing_{0}
    , childCount_(0)
    , inCount_(0)
    , outCount_(0)
    , loc_(dummylocation)
{
}

Node::Node(unsigned int id, TransportMode mode, LocationDatabase &ldb)
    : id_(id)
    , type_(mode)
    , isDest_(false)
    , isCatTranStop_(false)
    , isBikeDepot_(false)
    , children_{0}
    , incoming_{0}
    , outgoing_{0}
    , childCount_(0)
    , inCount_(0)
    , outCount_(0)
    , loc_(ldb)
{
}

Node::~Node() {}

Node::Node(const Node &copy)
    : loc_(copy.loc_)
{
  if (&copy != this)
  {
    this->id_ = copy.id_;
    this->type_ = copy.type_;
    this->isDest_ = copy.isDest_;
    this->isCatTranStop_ = copy.isCatTranStop_;
    this->isBikeDepot_ = copy.isBikeDepot_;
    this->childCount_ = copy.childCount_;
    this->inCount_ = copy.inCount_;
    this->outCount_ = copy.outCount_;

    memcpy(this->children_, copy.children_, sizeof(this->children_));
    memcpy(this->incoming_, copy.incoming_, sizeof(this->incoming_));
    memcpy(this->outgoing_, copy.outgoing_, sizeof(this->outgoing_));
  }
}

Node &Node::operator=(const Node &rhs)
{
  if (&rhs != this)
  {
    this->id_ = rhs.id_;
    this->type_ = rhs.type_;
    this->isDest_ = rhs.isDest_;
    this->isCatTranStop_ = rhs.isCatTranStop_;
    this->isBikeDepot_ = rhs.isBikeDepot_;
    this->childCount_ = rhs.childCount_;
    this->inCount_ = rhs.inCount_;
    this->outCount_ = rhs.outCount_;
    this->loc_ = rhs.loc_;

    memcpy(this->children_, rhs.children_, sizeof(this->children_));
    memcpy(this->incoming_, rhs.incoming_, sizeof(this->incoming_));
    memcpy(this->outgoing_, rhs.outgoing_, sizeof(this->outgoing_));
  }

  return *this;
}

void Node::setIsDest(bool isDest) { isDest_ = isDest; }

void Node::setIsCatTranStop(bool isCatTranStop) { isCatTranStop_ = isCatTranStop; }
void Node::setIsBikeDepot(bool isBikeDepot) { isBikeDepot_ = isBikeDepot; }

NodeStatus Node::addChild(Node *cNode)
{
  NodeStatus retVal(NodeStatus::UNKNOWN);

  if (MAX_EDGES > childCount_)
  {
    if (nullptr == children_[childCount_])
    {
      children_[childCount_] = cNode;
      ++childCount_;

      retVal = NodeStatus::NODE_OK;
    }
    else
    {
      retVal = NodeStatus::NODE_CHILD_NOT_NULL;
    }
  }
  else
  {
    retVal = NodeStatus::NODE_CHILDREN_FULL;
  }

  return retVal;
}

NodeStatus Node::addIncoming(Edge *inEdge)
{
  NodeStatus retVal(NodeStatus::UNKNOWN);

  if (MAX_EDGES > inCount_)
  {
    if (nullptr == incoming_[inCount_])
    {
      incoming_[inCount_] = inEdge;
      ++inCount_;

      retVal = NodeStatus::NODE_OK;
    }
    else
    {
      retVal = NodeStatus::NODE_INCOMING_NOT_NULL;
    }
  }
  else
  {
    retVal = NodeStatus::NODE_INCOMING_FULL;
  }

  return retVal;
}

NodeStatus Node::addOutgoing(Edge *outEdge)
{
  NodeStatus retVal(NodeStatus::UNKNOWN);

  if (MAX_EDGES > outCount_)
  {
    if (nullptr == outgoing_[outCount_])
    {
      outgoing_[outCount_] = outEdge;
      ++outCount_;

      retVal = NodeStatus::NODE_OK;
    }
    else
    {
      retVal = NodeStatus::NODE_OUTGOING_NOT_NULL;
    }
  }
  else
  {
    retVal = NodeStatus::NODE_OUTGOING_FULL;
  }

  return retVal;
}

unsigned int Node::id() { return id_; }

TransportMode Node::mode() { return type_; }

unsigned int Node::numChildren() { return childCount_; }

Node *Node::child(unsigned int index)
{
  Node *child(nullptr);

  if (MAX_EDGES > index)
  {
    child = children_[index];
  }

  return child;
}

Edge *Node::outgoing(unsigned int index)
{
  Edge *edge(nullptr);

  if (MAX_EDGES > index)
  {
    edge = outgoing_[index];
  }

  return edge;
}

// double Node::lineOfSight(unsigned int nId1, unsigned int nId2, NodeStatus & status)
//{
//     LocationDatabaseStatus ldbStatus(LocationDatabaseStatus::UNKNOWN);
//
//     double los(loc_.lineOfSight(nId1, nId2, ldbStatus));
//
//     if (LocationDatabaseStatus::LOCATION_OK == ldbStatus)
//     {
//         status = NodeStatus::NODE_OK;
//     }
//     else
//     {
//         status = NodeStatus::NODE_LOS_BAD_ID;
//     }
//
//     return los;
// }

void Node::print()
{
  printf("%2d (%s): ", id_, tModeStr(type_));

  if (true == isDest_) printf("Dest ");
  if (true == isCatTranStop_) printf("CalTranStop ");
  if (true == isBikeDepot_) printf("BikeDepot ");

  printf("- children (%d) ", childCount_);
  for (unsigned int i(0); i < childCount_; ++i)
  {
    if (0 == i)
    {
      printf("%u%c", children_[i]->id(), tModeChar(children_[i]->mode()));
    }
    else
    {
      printf(",%u%c", children_[i]->id(), tModeChar(children_[i]->mode()));
    }
  }

  printf(" - in (%d) ", inCount_);
  for (unsigned int i(0); i < inCount_; ++i)
  {
    if (0 == i)
    {
      printf("%u%c", (incoming_[i]->src())->id(), tModeChar((incoming_[i]->src())->mode()));
    }
    else
    {
      printf(",%u%c", (incoming_[i]->src())->id(), tModeChar((incoming_[i]->src())->mode()));
    }
  }

  printf(" - out (%d) ", outCount_);
  for (unsigned int i(0); i < outCount_; ++i)
  {
    if (0 == i)
    {
      printf("%u%c", (outgoing_[i]->dest())->id(), tModeChar((outgoing_[i]->dest())->mode()));
    }
    else
    {
      printf(",%u%c", (outgoing_[i]->dest())->id(), tModeChar((outgoing_[i]->dest())->mode()));
    }
  }
  printf("\n");
}

} // namespace graph
