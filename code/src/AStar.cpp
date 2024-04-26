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

#include <cstring>
#include <limits>
#include <stdio.h>
#include "AStar.h"

namespace astar
{

AStar::AStar(graph::Graph &g) : graph_(g), startId_(0), goalId_(0), costType_(CostType::UNKNOWN), hr_(0), min_(0.0), goal_(nullptr) {}

AStar::~AStar() {}

AStarStatus AStar::solve(unsigned int startId, unsigned int goalId, CostType costType, unsigned int hr, double min)
{
  AStarStatus retVal(AStarStatus::UNKNOWN);
  startId_ = startId;
  goalId_ = goalId;
  costType_ = costType;
  hr_ = hr;
  min_ = min;

  // auto start{ std::chrono::high_resolution_clock::now() };

  retVal = initialize();
  if (AStarStatus::ASTAR_OK == retVal)
  {
    retVal = run();
  }

  finalize();

  // auto end{ std::chrono::high_resolution_clock::now() };
  // std::chrono::duration<double> elapsedSec{ end - start };
  // printf("  Elapsed: %lld ns\n",
  //     std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedSec).count());

  return retVal;
}

AStarStatus AStar::initialize()
{
  AStarStatus retVal(AStarStatus::UNKNOWN);

  if ((true == open_.empty()) && (true == closed_.empty()) && (true == nodeList_.empty()))
  {
    struct AStarNode *n = createNode(startId_, TransportMode::NONE, retVal);
    if ((AStarStatus::ASTAR_OK == retVal) && (nullptr != n))
    {
      n->hr = hr_;
      n->min = min_;

      n->g = 0;
      n->h = heuritic((n->gNode)->id());
      n->f = n->g + n->h;

      open_.push_back(n);
      retVal = AStarStatus::ASTAR_OK;
    }
  }
  else
  {
    retVal = AStarStatus::ASTAR_INIT_FAILED;
  }

  return retVal;
}

void AStar::finalize()
{
  for (std::list<struct AStarNode *>::iterator it(nodeList_.begin()); it != nodeList_.end(); ++it)
  {
    if (*it != 0)
    {
      delete *it;
      *it = 0;
    }
  }

  open_.clear();
  closed_.clear();
  nodeList_.clear();

  startId_ = 0;
  goalId_ = 0;
}

AStarStatus AStar::run()
{
  AStarStatus retVal(AStarStatus::UNKNOWN);

  struct AStarNode *currNode(0);
  bool foundGoal(false);

  while ((false == foundGoal) && (false == open_.empty()))
  {
    //
    // 1. Find the lowest f in the open list, place it in the closed list
    currNode = open_.front();
    open_.pop_front();

    closed_.push_back(currNode);

    // 1a. If the current node is a goal node, end search
    if (goalId_ == (currNode->gNode)->id())
    {
      foundGoal = true;
      goal_ = currNode;
      break;
    }

    //
    // 2. Expand the node
    retVal = findChildren(currNode);

    //
    // 3. For each child
    struct AStarNode *child(nullptr);
    for (unsigned int i(0); i < (currNode->numChildren); ++i)
    {
      child = currNode->children[i];

      // 3a. If in the closed list, ignore
      if (true == isClosed(child))
      {
        continue;
      }

      // 3b. If a goal node, end search
      if (goalId_ == (child->gNode)->id())
      {
        foundGoal = true;
        goal_ = child;
        break;
      }

      // 3c. If not in the open list, calculate f, g, h, then add to the open list
      if (false == isOpen(child))
      {
        // f, g, h already updated in findChildren
        addToOpen(child);
      }

      // 3d. If in the open list, update it in the open list
      else
      {
        updateOpen(child);
      }
    }
  }

  if (true == foundGoal)
  {
    if (nullptr != goal_)
    {
      reconstructPath(goal_);
      retVal = AStarStatus::ASTAR_OK;
    }
    else
    {
      retVal = AStarStatus::ASTAR_GOAL_NULL;
      printf("ERROR: (AStar::run) goal_ is NULL\n");
    }
  }
  else
  {
    retVal = AStarStatus::ASTAR_FAILED_TO_FIND_GOAL;
    printf("ERROR: (AStar::run) Failed to find the goal state!\n");
  }

  return retVal;
}

struct AStar::AStarNode *AStar::createNode(unsigned int id, TransportMode mode, AStarStatus &status)
{
  status = AStarStatus::ASTAR_NODE_NOT_IN_GRAPH;
  struct AStarNode *n(nullptr);

  if (graph_.nodes_.find(std::make_pair(id, mode)) != graph_.nodes_.end())
  {
    n = createNode(graph_.nodes_.at(std::make_pair(id, mode)), status);
  }

  return n;
}

struct AStar::AStarNode *AStar::createNode(graph::Node *gNode, AStarStatus &status)
{
  struct AStarNode *n(nullptr);
  status = AStarStatus::ASTAR_NULL_GRAPH_NODE;

  if (nullptr != gNode)
  {
    n = new struct AStarNode;
    nodeList_.push_back(n);

    n->gNode = gNode;

    n->f = std::numeric_limits<double>::max();
    n->g = std::numeric_limits<double>::max();
    n->h = std::numeric_limits<double>::max();

    n->parent = nullptr;

    n->numChildren = 0;
    memset(n->children, 0, sizeof(n->children));

    status = AStarStatus::ASTAR_OK;
  }
  else
  {
    printf("ERROR: (AStar::createNode) graph::Node is null!\n");
  }

  return n;
}

double AStar::heuritic(unsigned int id)
{
  double cost(std::numeric_limits<double>::max());

  graph::GraphStatus status(graph::GraphStatus::UNKNOWN);
  double los(graph_.lineOfSight(id, goalId_, status));

  switch (costType_)
  {
  case CostType::DISTANCE:
    cost = los;
    break;

  case CostType::TIME_NO_TRAFFIC:
  case CostType::TIME_WITH_TRAFFIC:
    cost = los / (fastestMode() * KM_TO_MILE) * HR_TO_MIN;
    break;

  case CostType::UNKNOWN:
  default:
    printf("ERROR: (AStar::heuritic) Unknown cost type!\n");
  }

  return cost;
}

AStarStatus AStar::findChildren(struct AStarNode *n)
{
  AStarStatus retVal(AStarStatus::UNKNOWN);

  graph::Node *gNode(n->gNode);
  graph::Edge *gEdge(nullptr);

  n->numChildren = gNode->numChildren();

  for (unsigned int i(0); i < (n->numChildren); ++i)
  {
    struct AStarNode *c = createNode(gNode->child(i), retVal);
    if (AStarStatus::ASTAR_OK == retVal)
    {
      gEdge = gNode->outgoing(i);
      if ((nullptr != gEdge) && (gEdge->src() == n->gNode) && (gEdge->dest() == c->gNode))
      {
        graph::EdgeStatus eStatus(graph::EdgeStatus::UNKNOWN);
        c->g = n->g + gEdge->cost(costType_, n->hr, n->min, c->hr, c->min, eStatus);
        c->h = heuritic((c->gNode)->id());
        c->f = c->g + c->h;

        c->parent = n;

        n->children[i] = c;

        if (graph::EdgeStatus::EDGE_OK == eStatus)
        {
          retVal = AStarStatus::ASTAR_OK;
        }
        else
        {
          retVal = AStarStatus::ASTAR_EDGE_COST_FAILED;
          printf("ERROR: (AStar::findChildren) Edge cost failed\n");
        }
      }
    }
  }

  return retVal;
}

bool AStar::isOpen(struct AStarNode *n)
{
  for (std::list<struct AStarNode *>::iterator it(open_.begin()); it != open_.end(); ++it)
  {
    if (((*it)->gNode) == (n->gNode))
    {
      return true;
    }
  }

  return false;
}

bool AStar::isClosed(struct AStarNode *n)
{
  for (std::list<struct AStarNode *>::iterator it(closed_.begin()); it != closed_.end(); ++it)
  {
    if (((*it)->gNode) == (n->gNode))
    {
      return true;
    }
  }

  return false;
}

bool compare_node(AStar::AStarNode *n1, AStar::AStarNode *n2) { return n1->f < n2->f; }

void AStar::addToOpen(struct AStarNode *n)
{
  open_.push_back(n);
  open_.sort(compare_node);
}

void AStar::updateOpen(struct AStarNode *n)
{
  for (std::list<struct AStarNode *>::iterator it(open_.begin()); it != open_.end(); ++it)
  {
    if (((*it)->gNode) == (n->gNode))
    {
      if (((*it)->g) > (n->g))
      {
        open_.erase(it);
        addToOpen(n);
      }
      break;
    }
  }
}

void AStar::reconstructPath(struct AStarNode *n)
{
  solution_.clear();

  struct AStarNode *currNode(n);

  // To print the solution from the start node,
  // push parent nodes in the front of the solution list
  while (nullptr != currNode)
  {
    solution_.push_front(currNode);
    currNode = currNode->parent;
  }

  unsigned int order(1);
  for (std::list<struct AStarNode *>::iterator it(solution_.begin()); it != solution_.end(); ++it)
  {
    printf("  %2d:  %u%c  (%f)\n", order, ((*it)->gNode)->id(), tModeChar(((*it)->gNode)->mode()), (*it)->g);
    ++order;
  }
}

} // namespace astar
