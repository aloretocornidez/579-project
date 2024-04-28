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

#ifndef A_STAR_H
#define A_STAR_H

#include <list>
#include <string>

#include "./graph/Edge.h"
#include "./graph/Graph.h"
#include "./graph/Node.h"
#include "CommonUtilities.h"

namespace astar
{

enum class AStarStatus : unsigned int
{
  ASTAR_OK,
  ASTAR_NODE_NOT_IN_GRAPH,
  ASTAR_NULL_GRAPH_NODE,
  ASTAR_INIT_FAILED,
  ASTAR_EDGE_COST_FAILED,
  ASTAR_GOAL_NULL,
  ASTAR_FAILED_TO_FIND_GOAL,
  UNKNOWN
};

class AStar
{
public:
  static const unsigned int MAX_CHILDREN = 15u; // # of max possible children will change

  struct AStarNode
  {
    graph::Node *gNode;
    unsigned int hr;
    double min;

    double f;
    double g;
    double h;
    unsigned int numChildren;
    struct AStarNode *parent;
    struct AStarNode *children[MAX_CHILDREN];
  };

  AStar(graph::Graph &g);
  ~AStar();

  AStarStatus solve(unsigned int startId, unsigned int goalId, CostType costType = CostType::DISTANCE, unsigned int hr = 0, double min = 0.0);

private:
  AStarStatus initialize();
  void finalize();

  AStarStatus run();

  struct AStarNode *createNode(unsigned int id, TransportMode mode, AStarStatus &status);
  struct AStarNode *createNode(graph::Node *gNode, AStarStatus &status);

  double heuritic(unsigned int id);

  AStarStatus findChildren(struct AStarNode *n);

  bool isOpen(struct AStarNode *n);
  bool isClosed(struct AStarNode *n);

  void addToOpen(struct AStarNode *n);
  void updateOpen(struct AStarNode *n);

  std::string getCostString();

  void reconstructPath(struct AStarNode *n);

  graph::Graph &graph_;

  std::list<struct AStarNode *> open_;
  std::list<struct AStarNode *> closed_;

  std::list<struct AStarNode *> solution_;

  std::list<struct AStarNode *> nodeList_;

  unsigned int startId_;
  unsigned int goalId_;
  CostType costType_;
  unsigned int hr_;
  double min_;

  struct AStarNode *goal_;
};

} // namespace astar

#endif /* A_STAR_H */
