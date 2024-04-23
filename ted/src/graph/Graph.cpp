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
#include <limits>
#include <stdexcept>

#include "Graph.h"

namespace graph
{

Graph::Graph(LocationDatabase & ldb, PathDatabase & pdb, TrafficDatabase & tdb, CatTranDatabase & cdb)
    : loc_(ldb)
    , path_(pdb)
    , traffic_(tdb)
    , cattran_(cdb)
{
}

Graph::~Graph()
{
    for (std::list<Node *>::iterator it(nodeList_.begin());
        it != nodeList_.end(); ++it)
    {
        if (*it != 0)
        {
            delete * it;
            *it = 0;
        }
    }

    for (std::list<Edge *>::iterator it(edgeList_.begin());
        it != edgeList_.end(); ++it)
    {
        if (*it != 0)
        {
            delete * it;
            *it = 0;
        }
    }

    nodes_.clear();
    edges_.clear();
    nodeList_.clear();
    edgeList_.clear();
}

GraphStatus Graph::build()
{
    GraphStatus retVal(GraphStatus::UNKNOWN);

    retVal = buildNodes();
    if (GraphStatus::GRAPH_OK == retVal)
    {
        retVal = buildEdges();
        if (GraphStatus::GRAPH_OK == retVal)
        {
            retVal = finishBuild();
        }
    }

    return retVal;
}

GraphStatus Graph::buildNodes()
{
    GraphStatus retVal(GraphStatus::UNKNOWN);

    Node * n1(nullptr);
    Node * n2(nullptr);
    for (std::map<std::pair<unsigned int, unsigned int>, struct PathDatabase::PathDatabaseType>::iterator it(path_.db_.begin());
        it != path_.db_.end(); ++it)
    {
        switch ((it->second).mode)
        {
        case PathDatabase::PathDatabaseMode::WALK_BIKE_CATTRAN:
            {
                n1 = new Node((it->second).src, TransportMode::CATTRAN, loc_);
                nodeList_.push_back(n1);

                n2 = new Node((it->second).dest, TransportMode::CATTRAN, loc_);
                nodeList_.push_back(n2);

                nodes_[std::make_pair((it->second).src, TransportMode::CATTRAN)] = n1;
                nodes_[std::make_pair((it->second).dest, TransportMode::CATTRAN)] = n2;
            }

        case PathDatabase::PathDatabaseMode::WALK_BIKE:
            {
                n1 = new Node((it->second).src, TransportMode::BIKE, loc_);
                nodeList_.push_back(n1);

                n2 = new Node((it->second).dest, TransportMode::BIKE, loc_);
                nodeList_.push_back(n2);

                nodes_[std::make_pair((it->second).src, TransportMode::BIKE)] = n1;
                nodes_[std::make_pair((it->second).dest, TransportMode::BIKE)] = n2;
            }

        case PathDatabase::PathDatabaseMode::WALK:
            {
                n1 = new Node((it->second).src, TransportMode::WALK, loc_);
                nodeList_.push_back(n1);

                n2 = new Node((it->second).dest, TransportMode::WALK, loc_);
                nodeList_.push_back(n2);

                nodes_[std::make_pair((it->second).src, TransportMode::WALK)] = n1;
                nodes_[std::make_pair((it->second).dest, TransportMode::WALK)] = n2;
            }
            retVal = GraphStatus::GRAPH_OK;
            break;

        case PathDatabase::PathDatabaseMode::UNAVAILABLE:
        case PathDatabase::PathDatabaseMode::UNKNOWN:
        default:
            retVal = GraphStatus::GRAPH_BAD_PATH_MODE;
            printf("ERROR: (Graph::buildNodes) Bad path mode!\n");
        }
    }

    Node * n(nullptr);
    for (std::map<unsigned int, struct LocationDatabase::LocationDatabaseType>::iterator it(loc_.db_.begin());
        it != loc_.db_.end(); ++it)
    {
        if (true == (it->second).isDest)
        {
            n = new Node((it->second).id, TransportMode::NONE, loc_);
            nodeList_.push_back(n);

            nodes_[std::make_pair((it->second).id, TransportMode::NONE)] = n;
        }
    }

    return retVal;
}

GraphStatus Graph::buildEdges()
{
    GraphStatus retVal(GraphStatus::UNKNOWN);

    Node * src(nullptr);
    Node * dest(nullptr);
    Edge * e1(nullptr);
    Edge * e2(nullptr);
    for (std::map<std::pair<unsigned int, unsigned int>, struct PathDatabase::PathDatabaseType>::iterator it(path_.db_.begin());
        it != path_.db_.end(); ++it)
    {
        try
        {
            switch ((it->second).mode)
            {
            case PathDatabase::PathDatabaseMode::WALK_BIKE_CATTRAN:
                {
                    struct LocationDatabase::LocationDatabaseType & locSrc(loc_.db_.at((it->second).src));

                    //if (true == locSrc.isDest)
                    //{
                    //    src = nodes_.at(std::make_pair((it->second).src, TransportMode::NONE));
                    //}
                    //else
                    //{
                    //    src = nodes_.at(std::make_pair((it->second).src, TransportMode::CATTRAN));
                    //}

                    src = nodes_.at(std::make_pair((it->second).src, TransportMode::CATTRAN));
                    dest = nodes_.at(std::make_pair((it->second).dest, TransportMode::CATTRAN));

                    e1 = new Edge(src, dest, TransportMode::CATTRAN, path_, traffic_, cattran_);
                    edgeList_.push_back(e1);

                    src = nodes_.at(std::make_pair((it->second).src, TransportMode::CATTRAN));

                    e2 = new Edge(dest, src, TransportMode::CATTRAN, path_, traffic_, cattran_);
                    edgeList_.push_back(e2);

                    edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::CATTRAN)] = e1;
                    edges_[std::make_tuple(toNodeCode(dest->id(), dest->mode()), toNodeCode(src->id(), src->mode()), TransportMode::CATTRAN)] = e2;
                }

            case PathDatabase::PathDatabaseMode::WALK_BIKE:
                {
                    struct LocationDatabase::LocationDatabaseType & locSrc(loc_.db_.at((it->second).src));

                    //if (true == locSrc.isDest)
                    //{
                    //    src = nodes_.at(std::make_pair((it->second).src, TransportMode::NONE));
                    //}
                    //else
                    //{
                    //    src = nodes_.at(std::make_pair((it->second).src, TransportMode::BIKE));
                    //}

                    src = nodes_.at(std::make_pair((it->second).src, TransportMode::BIKE));
                    dest = nodes_.at(std::make_pair((it->second).dest, TransportMode::BIKE));

                    e1 = new Edge(src, dest, TransportMode::BIKE, path_, traffic_, cattran_);
                    edgeList_.push_back(e1);

                    src = nodes_.at(std::make_pair((it->second).src, TransportMode::BIKE));

                    e2 = new Edge(dest, src, TransportMode::BIKE, path_, traffic_, cattran_);
                    edgeList_.push_back(e2);

                    edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::CATTRAN)] = e1;
                    edges_[std::make_tuple(toNodeCode(dest->id(), dest->mode()), toNodeCode(src->id(), src->mode()), TransportMode::CATTRAN)] = e2;
                }

            case PathDatabase::PathDatabaseMode::WALK:
                {
                    struct LocationDatabase::LocationDatabaseType & locSrc(loc_.db_.at((it->second).src));

                    //if (true == locSrc.isDest)
                    //{
                    //    src = nodes_.at(std::make_pair((it->second).src, TransportMode::NONE));
                    //}
                    //else
                    //{
                    //    src = nodes_.at(std::make_pair((it->second).src, TransportMode::WALK));
                    //}

                    src = nodes_.at(std::make_pair((it->second).src, TransportMode::WALK));
                    dest = nodes_.at(std::make_pair((it->second).dest, TransportMode::WALK));

                    e1 = new Edge(src, dest, TransportMode::WALK, path_, traffic_, cattran_);
                    edgeList_.push_back(e1);

                    src = nodes_.at(std::make_pair((it->second).src, TransportMode::WALK));

                    e2 = new Edge(dest, src, TransportMode::WALK, path_, traffic_, cattran_);
                    edgeList_.push_back(e2);

                    edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::CATTRAN)] = e1;
                    edges_[std::make_tuple(toNodeCode(dest->id(), dest->mode()), toNodeCode(src->id(), src->mode()), TransportMode::CATTRAN)] = e2;
                }

            retVal = GraphStatus::GRAPH_OK;
            break;

            case PathDatabase::PathDatabaseMode::UNAVAILABLE:
            case PathDatabase::PathDatabaseMode::UNKNOWN:
            default:
                retVal = GraphStatus::GRAPH_BAD_PATH_MODE;
                printf("ERROR: (Graph::buildEdges) Bad path mode!\n");
            }
        }
        catch (const std::out_of_range & oor)
        {
            (void)oor;
            retVal = GraphStatus::GRAPH_NOT_IN_LOCATION_DB;
            printf("ERROR: (Graph::buildEdges) src does not in the Location DB!\n");
        }
    }

    Edge * e(nullptr);
    for (std::map<unsigned int, struct LocationDatabase::LocationDatabaseType>::iterator it(loc_.db_.begin());
        it != loc_.db_.end(); ++it)
    {
        try
        {
            if (true == (it->second).isDest)
            {
                unsigned int nId1(0);
                unsigned int nId2(0);
                for (unsigned int i(0); i < (it->second).numNeighbors; ++i)
                {
                    nId1 = ((it->second).id < (it->second).neighbor[i]) ? (it->second).id : (it->second).neighbor[i];
                    nId2 = ((it->second).id < (it->second).neighbor[i]) ? (it->second).neighbor[i] : (it->second).id;

                    struct PathDatabase::PathDatabaseType & p(path_.db_.at(std::make_pair(nId1, nId2)));

                    switch (p.mode)
                    {
                    case PathDatabase::PathDatabaseMode::WALK_BIKE_CATTRAN:
                    {
                        // NONE --> CATTRAN
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::NONE));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::CATTRAN));

                        e = new Edge(src, dest, TransportMode::CATTRAN, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::CATTRAN)] = e;
                    }

                    case PathDatabase::PathDatabaseMode::WALK_BIKE:
                    {
                        // NONE --> BIKE
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::NONE));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::BIKE));

                        e = new Edge(src, dest, TransportMode::BIKE, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::BIKE)] = e;
                    }

                    case PathDatabase::PathDatabaseMode::WALK:
                    {
                        // NONE --> WALK
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::NONE));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::WALK));

                        e = new Edge(src, dest, TransportMode::WALK, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::WALK)] = e;
                    }

                    retVal = GraphStatus::GRAPH_OK;
                    break;

                    case PathDatabase::PathDatabaseMode::UNKNOWN:
                    case PathDatabase::PathDatabaseMode::UNAVAILABLE:
                    default:
                        retVal = GraphStatus::GRAPH_BAD_PATH_MODE;
                        printf("ERROR: (Graph::buildEdges) Bad path mode!\n");
                    }
                }
            }
        }
        catch (const std::out_of_range & oor)
        {
            (void)oor;
            retVal = GraphStatus::GRAPH_NOT_IN_PATH_DB;
            printf("ERROR: (Graph::buildEdges) src does not in the Path DB!\n");
        }
    }

    for (std::map<unsigned int, struct LocationDatabase::LocationDatabaseType>::iterator it(loc_.db_.begin());
        it != loc_.db_.end(); ++it)
    {
        try
        {
            if ((true == (it->second).isCatTranStop) && (true == (it->second).isBikeDepot))
            {
                unsigned int nId1(0);
                unsigned int nId2(0);
                for (unsigned int i(0); i < (it->second).numNeighbors; ++i)
                {
                    nId1 = ((it->second).id < (it->second).neighbor[i]) ? (it->second).id : (it->second).neighbor[i];
                    nId2 = ((it->second).id < (it->second).neighbor[i]) ? (it->second).neighbor[i] : (it->second).id;

                    struct PathDatabase::PathDatabaseType & p(path_.db_.at(std::make_pair(nId1, nId2)));

                    switch (p.mode)
                    {
                    case PathDatabase::PathDatabaseMode::WALK_BIKE_CATTRAN:
                    {
                        // BIKE --> CATTRAN
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::BIKE));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::CATTRAN));

                        e = new Edge(src, dest, TransportMode::CATTRAN, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::CATTRAN)] = e;

                        // WALK --> CATTRAN
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::WALK));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::CATTRAN));

                        e = new Edge(src, dest, TransportMode::CATTRAN, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::CATTRAN)] = e;
                    }

                    case PathDatabase::PathDatabaseMode::WALK_BIKE:
                    {
                        // CATTRAN --> BIKE
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::CATTRAN));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::BIKE));

                        e = new Edge(src, dest, TransportMode::BIKE, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::BIKE)] = e;

                        // WALK --> BIKE
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::WALK));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::BIKE));

                        e = new Edge(src, dest, TransportMode::BIKE, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::BIKE)] = e;
                    }

                    case PathDatabase::PathDatabaseMode::WALK:
                    {
                        // CATTRAN --> WALK
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::CATTRAN));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::WALK));

                        e = new Edge(src, dest, TransportMode::WALK, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::WALK)] = e;

                        // BIKE --> WALK
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::BIKE));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::WALK));

                        e = new Edge(src, dest, TransportMode::WALK, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::WALK)] = e;
                    }

                    retVal = GraphStatus::GRAPH_OK;
                    break;

                    case PathDatabase::PathDatabaseMode::UNKNOWN:
                    case PathDatabase::PathDatabaseMode::UNAVAILABLE:
                    default:
                        retVal = GraphStatus::GRAPH_BAD_PATH_MODE;
                        printf("ERROR: (Graph::buildEdges) Bad path mode!\n");
                    }
                }
            }

            else if (true == (it->second).isCatTranStop)
            {
                unsigned int nId1(0);
                unsigned int nId2(0);
                for (unsigned int i(0); i < (it->second).numNeighbors; ++i)
                {
                    nId1 = ((it->second).id < (it->second).neighbor[i]) ? (it->second).id : (it->second).neighbor[i];
                    nId2 = ((it->second).id < (it->second).neighbor[i]) ? (it->second).neighbor[i] : (it->second).id;

                    struct PathDatabase::PathDatabaseType & p(path_.db_.at(std::make_pair(nId1, nId2)));

                    switch (p.mode)
                    {
                    case PathDatabase::PathDatabaseMode::WALK_BIKE_CATTRAN:
                    {
                        // WALK --> CATTRAN
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::WALK));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::CATTRAN));

                        e = new Edge(src, dest, TransportMode::CATTRAN, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::CATTRAN)] = e;
                    }

                    case PathDatabase::PathDatabaseMode::WALK_BIKE:
                    case PathDatabase::PathDatabaseMode::WALK:
                    {
                        // CATTRAN --> WALK
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::CATTRAN));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::WALK));

                        e = new Edge(src, dest, TransportMode::WALK, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::WALK)] = e;
                    }

                    retVal = GraphStatus::GRAPH_OK;
                    break;

                    case PathDatabase::PathDatabaseMode::UNKNOWN:
                    case PathDatabase::PathDatabaseMode::UNAVAILABLE:
                    default:
                        retVal = GraphStatus::GRAPH_BAD_PATH_MODE;
                        printf("ERROR: (Graph::buildEdges) Bad path mode!\n");
                    }
                }
            }

            else if (true == (it->second).isBikeDepot)
            {
                unsigned int nId1(0);
                unsigned int nId2(0);
                for (unsigned int i(0); i < (it->second).numNeighbors; ++i)
                {
                    nId1 = ((it->second).id < (it->second).neighbor[i]) ? (it->second).id : (it->second).neighbor[i];
                    nId2 = ((it->second).id < (it->second).neighbor[i]) ? (it->second).neighbor[i] : (it->second).id;

                    struct PathDatabase::PathDatabaseType & p(path_.db_.at(std::make_pair(nId1, nId2)));

                    switch (p.mode)
                    {
                    case PathDatabase::PathDatabaseMode::WALK_BIKE_CATTRAN:
                    case PathDatabase::PathDatabaseMode::WALK_BIKE:
                    {
                        // WALK --> BIKE
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::WALK));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::BIKE));

                        e = new Edge(src, dest, TransportMode::BIKE, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::BIKE)] = e;
                    }

                    case PathDatabase::PathDatabaseMode::WALK:
                    {
                        // BIKE --> WALK
                        src = nodes_.at(std::make_pair((it->second).id, TransportMode::BIKE));
                        dest = nodes_.at(std::make_pair((it->second).neighbor[i], TransportMode::WALK));

                        e = new Edge(src, dest, TransportMode::WALK, path_, traffic_, cattran_);
                        edgeList_.push_back(e);

                        edges_[std::make_tuple(toNodeCode(src->id(), src->mode()), toNodeCode(dest->id(), dest->mode()), TransportMode::WALK)] = e;
                    }

                    retVal = GraphStatus::GRAPH_OK;
                    break;

                    case PathDatabase::PathDatabaseMode::UNKNOWN:
                    case PathDatabase::PathDatabaseMode::UNAVAILABLE:
                    default:
                        retVal = GraphStatus::GRAPH_BAD_PATH_MODE;
                        printf("ERROR: (Graph::buildEdges) Bad path mode!\n");
                    }
                }
            }
        }
        catch (const std::out_of_range & oor)
        {
            (void)oor;
            retVal = GraphStatus::GRAPH_NOT_IN_PATH_DB;
            printf("ERROR: (Graph::buildEdges) src does not in the Path DB!\n");
        }
    }

    return retVal;
}

GraphStatus Graph::finishBuild()
{
    GraphStatus retVal(GraphStatus::UNKNOWN);

    Node * src(nullptr);
    Node * dest(nullptr);

    for (std::map<std::tuple<unsigned int, unsigned int, TransportMode>, Edge *>::iterator it(edges_.begin());
        it != edges_.end(); ++it)
    {
        src = (it->second)->src();
        dest = (it->second)->dest();

        src->addChild(dest);
        src->addOutgoing(it->second);
        dest->addIncoming(it->second);
    }

    try
    {
        for (std::map<std::pair<unsigned int, TransportMode>, Node *>::iterator it(nodes_.begin());
            it != nodes_.end(); ++it)
        {
            struct LocationDatabase::LocationDatabaseType & loc(loc_.db_.at((it->second)->id()));

            (it->second)->setIsDest(loc.isDest);
            (it->second)->setIsCatTranStop(loc.isCatTranStop);
            (it->second)->setIsBikeDepot(loc.isBikeDepot);
        }

        retVal = GraphStatus::GRAPH_OK;
    }
    catch (const std::out_of_range & oor)
    {
        (void)oor;
        retVal = GraphStatus::GRAPH_NOT_IN_LOCATION_DB;
        printf("ERROR: (Graph::buildEdges) src does not in the Location DB!\n");
    }

    return retVal;
}

double Graph::lineOfSight(unsigned int nId1, unsigned int nId2, GraphStatus & status)
{
    LocationDatabaseStatus ldbStatus(LocationDatabaseStatus::UNKNOWN);

    double los(loc_.lineOfSight(nId1, nId2, ldbStatus));

    if (LocationDatabaseStatus::LOCATION_OK == ldbStatus)
    {
        status = GraphStatus::GRAPH_OK;
    }
    else
    {
        status = GraphStatus::GRAPH_LOS_BAD_ID;
    }

    return los;
}

void Graph::print(unsigned int id)
{
    for (std::map<std::pair<unsigned int, TransportMode>, Node *>::iterator it(nodes_.begin());
        it != nodes_.end(); ++it)
    {
        if ((ALL == id) || ((it->second)->id() == id))
        {
            (it->second)->print();
        }
    }

    for (std::map<std::tuple<unsigned int, unsigned int, TransportMode>, Edge *>::iterator it(edges_.begin());
        it != edges_.end(); ++it)
    {
        if ((ALL == id) || (((it->second)->src())->id() == id) || (((it->second)->dest())->id() == id))
        {
            (it->second)->print();
        }
    }
}

} // namespace graph