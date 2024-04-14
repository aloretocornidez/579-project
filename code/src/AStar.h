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

#include <algorithm>
#include <list>

#include "./db/LocationDatabase.h"
#include "./db/PathDatabase.h"

enum class AStarStatus : unsigned int
{
  ASTAR_OK,
  ASTAR_INIT_FAILED,
  ASTAR_GOAL_NULL,
  ASTAR_FAILED_TO_FIND_GOAL,
  UNKNOWN
};

// Used to calculate new cost of nodes.
enum class ArrivalMethod : unsigned int
{
  WALKED,
  BIKED,
  CAT_TRANNED,
  UNKNOWN
};

class AStar
{
public:
  static const unsigned int MAX_CHILDREN = 6; // # of max possible children will change

  // TODO: Add an implementation for a traveler (this will keep track of
  // available options for the A-Star algorithm to find a solution path.)

  typedef struct Node_s
  {
    // Used to identify the node in the search tree.
    unsigned int node_id;
    // Used to id the physical location that the node pertains to.
    unsigned int location_id;

    // Heuristic information.
    double f;
    double g;
    double h;

    // Transportation methods available.
    ArrivalMethod arrivalMethod;

    // unsigned int numDestinations;
    Node_s *parent;
    Node_s *children[MAX_CHILDREN];

  } Node;

  AStar(PathDatabase &eDb, LocationDatabase &nDb);
  ~AStar();

  AStarStatus solve(unsigned int startId, unsigned int goalId);

private:
  AStarStatus initialize();
  void finalize();

  AStarStatus run();

  Node *createNode(unsigned int id, AStarStatus &status);

  double heuristic(unsigned int id);

  AStarStatus findChildren(Node *n);

  bool isOpen(Node *n);
  bool isClosed(Node *n);

  void addToOpen(Node *n);
  void updateOpen(Node *n);

  void reconstructPath(Node *n);

  // Pathways (Edges)
  PathDatabase &path_;
  // Locations (Nodes)
  LocationDatabase &loc_;

  // Open queue.
  std::list<Node *> open_;
  // Closed queue.
  std::list<Node *> closed_;

  // Solution path.
  std::list<Node *> solution_;

  // Keep track of all allocated memory.
  std::list<Node *> nodeList_;

  // Start nodes
  unsigned int startId_;
  // Goal nodes
  unsigned int goalId_;

  // Pointer to the goal node
  Node *goal_;

  unsigned int node_id_counter;

  ArrivalMethod traveller_;
};

#endif /* A_STAR_H */
