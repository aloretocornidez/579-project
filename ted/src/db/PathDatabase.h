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

#ifndef PATH_DATABASE_H
#define PATH_DATABASE_H

#include <map>

enum class PathDatabaseStatus : unsigned int
{
    PATH_OK,
    PATH_FILE_OPEN_FAILED,
    PATH_BAD_FORMAT,
    PATH_INVALID_MODE,
    UNKNOWN
};

class PathDatabase
{
public:
    enum class PathDatabaseMode : unsigned int
    {
        UNAVAILABLE,
        WALK,
        WALK_BIKE,
        WALK_BIKE_CATTRAN,
        UNKNOWN
    };

    struct PathDatabaseType
    {
        unsigned int src;
        unsigned int dest;
        double dist_mile;
        double straight_mile;
        PathDatabaseMode mode;
        unsigned int trafficRating;
    };

    PathDatabase();
    ~PathDatabase();

    PathDatabaseStatus read(const char * filename);
    void print();

    std::map<std::pair<unsigned int, unsigned int>, struct PathDatabaseType> db_;
    //std::map<std::pair<unsigned int, unsigned int>, struct PathDatabaseType> rdb_;

private:
    static constexpr double FT_IN_MILE = 5280.0;

};

#endif /* PATH_DATABASE_H */
