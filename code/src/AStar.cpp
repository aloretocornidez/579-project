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

#include "AStar.h"
#include <cstring>
#include <limits>
#include <stdexcept>
#include <stdio.h>

AStar::AStar(PathDatabase &eDb, LocationDatabase &nDb) : path_(eDb), loc_(nDb), startId_(0), goalId_(0), goal_(nullptr), node_id_counter(0) {}

AStar::~AStar() {}

AStarStatus AStar::solve(unsigned int startId, unsigned int goalId)
{
  AStarStatus retVal(AStarStatus::UNKNOWN);
  startId_ = startId;
  goalId_ = goalId;

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
  // std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedSec).count());

  return retVal;
}

AStarStatus AStar::initialize()
{
  // Return Status
  AStarStatus retVal(AStarStatus::UNKNOWN);

  // If the queues are empty, initialize them.
  if (open_.empty() && closed_.empty() && nodeList_.empty())
  {
    // Create the initial node with a given location ID
    Node *n = createNode(startId_, retVal);
    if ((AStarStatus::ASTAR_OK == retVal) && (nullptr != n))
    {
      n->g = 0;
      n->h = heuristic(n->location_id);
      n->f = n->g + n->h;

      open_.push_back(n);
      retVal = AStarStatus::ASTAR_OK;
    }
  }
  // Queues were not empty, error.
  else
  {
    retVal = AStarStatus::ASTAR_INIT_FAILED;
  }

  return retVal;
}

void AStar::finalize()
{
  for (std::list<Node *>::iterator it(nodeList_.begin()); it != nodeList_.end(); ++it)
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

  Node *currNode(0);
  bool foundGoal(false);

  while ((false == foundGoal) && (false == open_.empty()))
  {
    // 1. Find the lowest f in the open list, place it in the closed list
    currNode = open_.front();  //TO DO: Where is the open queue being sorted?
    open_.pop_front();

    closed_.push_back(currNode);

    // 1a. If the current node is a goal node, end search
    if (goalId_ == currNode->location_id)
    {
      foundGoal = true;
      goal_ = currNode;
      break;
    }

    // 2. Expand the node
    retVal = findChildren(currNode);

    // 3. For each child
    // TO DO: Refactor this based on output of updated findChildren() method
    Node *child(nullptr);
    for (unsigned int i(0); i < (currNode->numDestinations); ++i)
    {
      child = currNode->children[i];

      // 3a. If in the closed list, ignore
      if (true == isClosed(child))
      {
        continue;
      }

      // 3b. If a goal node, end search
      if (goalId_ == child->location_id)  //Huh? Don't think this check should be here.
      {
        foundGoal = true;
        goal_ = child;
        break;
      }

      // 3c. If not in the open list, calculate f, g, h, then add to the
      // open list
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

AStar::Node *AStar::createNode(unsigned int id, AStarStatus &status)
{
  status = AStarStatus::UNKNOWN;

  // Create a temporary node initialized with nullptr.
  Node *n(nullptr);

  // TODO: Make sure to take into account, the ID of the location being
  // separate from the node ID in the search tree of the algorithm.

  // If the id is not the end of the database, proceed.
  if (loc_.db_.find(id) != loc_.db_.end())
  {
    // Create a new node.
    n = new Node;

    // Add the generated node to keep track of memory.
    nodeList_.push_back(n);

    // Update the location_id of the created node.
    n->location_id = loc_.db_[id].location_id;
    // Update the ID of the created node.
    n->node_id = node_id_counter++;

    // Initialize to max possible cost.
    n->f = std::numeric_limits<double>::max();
    n->g = std::numeric_limits<double>::max();
    n->h = std::numeric_limits<double>::max();

    // Initialize a 0 value for the parent.
    n->parent = 0;
    
    // Initializing unknown arrival method.
    n->arrivalMethod = ArrivalMethod::UNKNOWN;

    // Initialize the number of children to 0.
    // n->numDestinations = 0; // Removed for now.

    // Make sure the rest of the data is set to 0.
    memset(n->children, 0, sizeof(n->children));

    // Set an okay return status.
    status = AStarStatus::ASTAR_OK;
  }

  // Return the address of the created node.
  return n;
}

double AStar::heuristic(unsigned int id)
{
  LocationDatabaseStatus status(LocationDatabaseStatus::UNKNOWN);
  return loc_.lineOfSight(id, goalId_, status);
}

// TO DO: Rework this function using Elliot's psuedocode
AStarStatus AStar::findChildren(Node *n)
{
  // Return value for success in child generation.
  AStarStatus retVal(AStarStatus::UNKNOWN);

  // Find the data of the node that was passed as input.
  struct LocationDatabase::Location &nData(loc_.db_[n->location_id]);

  // id is used to populate the edge's id fields.
  unsigned int id1(0);
  unsigned int id2(0);

  // Get the possible number of children for the current node.
  // n->numDestinations = nData.num_outbound_paths;

  //
  for (unsigned int i(0); i < nData.num_outbound_paths; ++i)
  {
    Node *c = createNode(nData.outbound_paths[i], retVal);

    // If the node is created successfully.
    if (AStarStatus::ASTAR_OK == retVal)
    {
      try
      {
        // Edge DB map key ID pair needs to have smaller ID first
        id1 = (n->node_id < c->node_id) ? n->node_id : c->node_id;
        id2 = (n->node_id < c->node_id) ? c->node_id : n->node_id;

        // std::map::at throws out_of_range exception
        struct PathDatabase::PathDatabaseType &eData = path_.db_.at(std::make_pair(id1, id2));

        c->g = n->g + eData.dist_mile;
        c->h = heuristic(c->node_id);
        c->f = c->g + c->h;

        c->parent = n;

        n->children[i] = c;

        retVal = AStarStatus::ASTAR_OK;
      }
      catch (const std::out_of_range &oor)
      {
        (void)oor;
        printf("ERROR: (AStar::findChildren) Edge does not exist!");
      }
    }
  }

  return retVal;
}

//TO DO: it will be necessary to check ALL node properties except node_id to detect equivalence
bool AStar::isOpen(Node *n)
{
  for (std::list<Node *>::iterator it(open_.begin()); it != open_.end(); ++it)
  {
    if (((*it)->location_id) == (n->location_id))
    {
      return true;
    }
  }

  return false;
}

//TO DO: it will be necessary to check ALL node properties except node_id to detect equivalence
bool AStar::isClosed(Node *n)
{
  for (std::list<Node *>::iterator it(closed_.begin()); it != closed_.end(); ++it)
  {
    if (((*it)->node_id) == (n->node_id))
    {
      return true;
    }
  }

  return false;
}

bool compare_node(AStar::Node *n1, AStar::Node *n2) { return n1->f < n2->f; }

void AStar::addToOpen(Node *n)
{
  open_.push_back(n);
  open_.sort(compare_node);
}

//TO DO: it will be necessary to check ALL node properties except node_id to detect equivalence
void AStar::updateOpen(Node *n)
{
  for (std::list<Node *>::iterator it(open_.begin()); it != open_.end(); ++it)
  {
    if (((*it)->node_id) == (n->node_id))
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

void AStar::reconstructPath(Node *n)
{
  solution_.clear();

  Node *currNode = n;

  // To print the solution from the start node,
  // push parent nodes in the front of the solution list
  while (0 != currNode)
  {
    solution_.push_front(currNode);
    currNode = currNode->parent;
  }

  unsigned int order(1);
  for (std::list<Node *>::iterator it(solution_.begin()); it != solution_.end(); ++it)
  {
    // printf("  %2d:  %d  (%f)\n", order, (*it)->id, (*it)->g);
    ++order;
  }
}
