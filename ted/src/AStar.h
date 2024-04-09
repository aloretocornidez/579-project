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

#include "EdgeDatabase.h"
#include "NodeDatabase.h"

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

    struct Node
    {
        unsigned int id;
        double f;
        double g;
        double h;
        unsigned int numChildren;
        struct Node * parent;
        struct Node * children[MAX_CHILDREN];
    };

    AStar(EdgeDatabase & eDb, NodeDatabase & nDb);
    ~AStar();

    AStarStatus solve(unsigned int startId, unsigned int goalId);

private:
    AStarStatus initialize();
    void finalize();

    AStarStatus run();

    struct Node * createNode(unsigned int id, AStarStatus & status);

    double heuritic(unsigned int id);

    AStarStatus findChildren(struct Node * n);

    bool isOpen(struct Node * n);
    bool isClosed(struct Node * n);

    void addToOpen(struct Node * n);
    void updateOpen(struct Node * n);

    void reconstructPath(struct Node * n);

    EdgeDatabase & path_;
    NodeDatabase & loc_;

    std::list<struct Node *> open_;
    std::list<struct Node *> closed_;

    std::list<struct Node *> solution_;

    std::list<struct Node *> nodeList_;

    unsigned int startId_;
    unsigned int goalId_;

    struct Node * goal_;

};

#endif /* A_STAR_H */
