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
#include <fstream>
#include <sstream>
#include <string>

#include "CatTranDatabase.h"

CatTranDatabase::CatTranDatabase()
{
}

CatTranDatabase::~CatTranDatabase()
{
    if (true == db_.empty())
    {
        db_.clear();
    }
}

CatTranDatabaseStatus CatTranDatabase::read(const char * filename)
{
    CatTranDatabaseStatus retVal(CatTranDatabaseStatus::UNKNOWN);

    std::fstream f;
    f.open(filename, std::fstream::in);
    if (true == f.good())
    {
        struct CatTranDatabaseType c = {0};
        unsigned int stopHr[NUM_SAMPLES] = {0};
        unsigned int stopMin[NUM_SAMPLES] = {0};
        char comma(0);
        char colon(0);

        std::string line;
        while (std::getline(f, line))
        {
            //printf("%s\n", line.c_str());

            std::stringstream ss(line);
            
            if (!(ss >> c.locId))  retVal = CatTranDatabaseStatus::CATTRAN_BAD_FORMAT;
            
            for (unsigned int i(0); i < NUM_SAMPLES; ++i)
            {
                if (!(ss >> comma >> stopHr[i] >> colon >> stopMin[i]))
                {
                    retVal = CatTranDatabaseStatus::CATTRAN_BAD_FORMAT;
                }
            }
            
            c.hr = 7;
            c.min = 0;
            
            unsigned int n(0);
            unsigned int tMin(0);
            unsigned int sMin(0);
            for (unsigned int t(0); t < TOTAL_MIN; ++t)
            {
                tMin = c.hr * 60 + c.min;
                sMin = stopHr[n] * 60 + stopMin[n];
                
                c.minToNext = sMin - tMin;
                
                db_[std::make_tuple(c.locId, c.hr, c.min)] = c;
                
                if (0 == c.minToNext) ++n;
                
                ++c.min;
                if (60 == c.min)
                {
                    ++c.hr;
                    c.min = 0;
                }
            }
            
            //db_[c.locId] = c;
        }

        if (CatTranDatabaseStatus::UNKNOWN == retVal)
        {
            retVal = CatTranDatabaseStatus::CATTRAN_OK;
        }
    }
    else
    {
        printf("ERROR: (CatTranDatabase::read) fstream::open failed!\n");
        return CatTranDatabaseStatus::CATTRAN_FILE_OPEN_FAILED;
    }

    f.close();

    return retVal;
}

void CatTranDatabase::print()
{
//    for (std::map<std::tuple<unsigned int, unsigned int, unsigned int>, struct CatTranDatabaseType>::iterator it(db_.begin());
//         it != db_.end(); ++it)
//    {
//        printf("%02u: (%02u:%02u) %02u\n",
//               (it->second).locId, (it->second).hr, (it->second).min, (it->second).minToNext);
//    }
    
    unsigned int loc[6] = {12, 27, 55, 40, 20, 4};
    unsigned int hr(7);
    unsigned int min(0);
    
    for (unsigned int i(0); i < TOTAL_MIN; ++i)
    {
        printf("%02u:%02u - ", hr, min);
        
        unsigned int n;
        for (n = 0; n < 5; ++n)
        {
            printf("(%02u) %02u, ", loc[n], db_[std::make_tuple(loc[n], hr, min)].minToNext);
        }
        printf("(%02u) %02u\n", loc[n], db_[std::make_tuple(loc[n], hr, min)].minToNext);
        
        ++min;
        if (60 == min)
        {
            ++hr;
            min = 0;
        }
    }
}
