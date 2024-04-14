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

#include "./db/EdgeDatabase.h"
#include "./db/NodeDatabase.h"

enum class AStarStatus : unsigned int
{
  ASTAR_OK,
  ASTAR_INIT_FAILED,
  ASTAR_GOAL_NULL,
  ASTAR_FAILED_TO_FIND_GOAL,
  UNKNOWN
};

class AStar
{
public:
  static const unsigned int MAX_CHILDREN = 6; // # of max possible children will change

  // TODO: Add an implementation for a traveler (this will keep track of
  // available options for the A-Star algorithm to find a solution path.)
  typedef struct Traveller
  {
    bool canWalk;
    bool canBike;
    bool canCatTran;
  } Traveller;

  typedef struct Location
  {
    unsigned int id;
    double f;
    double g;
    double h;
    unsigned int numDestinations;
    Location *parent;
    Location *children[MAX_CHILDREN];
  } Location;

  AStar(PathDatabase &eDb, NodeDatabase &nDb, Traveller &traveller);
  AStar(PathDatabase &eDb, NodeDatabase &nDb);
  ~AStar();

  AStarStatus solve(unsigned int startId, unsigned int goalId);

private:
  AStarStatus initialize();
  void finalize();

  AStarStatus run();

  Location *createNode(unsigned int id, AStarStatus &status);

  double heuristic(unsigned int id);

  AStarStatus findChildren(Location *n);

  bool isOpen(Location *n);
  bool isClosed(Location *n);

  void addToOpen(Location *n);
  void updateOpen(Location *n);

  void reconstructPath(Location *n);

  // Pathways (Edges)
  PathDatabase &path_;
  // Locations (Nodes)
  NodeDatabase &loc_;

  // Open queue.
  std::list<Location *> open_;
  // Closed queue.
  std::list<Location *> closed_;

  // Solution path.
  std::list<Location *> solution_;

  // Keep track of all allocated memory.
  std::list<Location *> nodeList_;

  // Start nodes
  unsigned int startId_;
  // Goal nodes
  unsigned int goalId_;

  // Pointer to the goal node
  Location *goal_;

  Traveller traveller_;
};

#endif /* A_STAR_H */
