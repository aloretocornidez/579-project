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

#ifndef CATTRAN_DATABASE_H
#define CATTRAN_DATABASE_H

#include <map>

enum class CatTranDatabaseStatus : unsigned int
{
  CATTRAN_OK,
  CATTRAN_FILE_OPEN_FAILED,
  CATTRAN_BAD_FORMAT,
  UNKNOWN
};

class CatTranDatabase
{
public:
  static constexpr unsigned int TOTAL_MIN = 661u; // Number of min from 7:00 to 18:00

  struct CatTranDatabaseType
  {
    unsigned int locId;
    unsigned int hr;
    unsigned int min;
    unsigned int minToNext; // Number of min to the next bus
  };

  CatTranDatabase();
  ~CatTranDatabase();

  CatTranDatabaseStatus read(const char *filename);
  void print();

  std::map<std::tuple<unsigned int, unsigned int, unsigned int>, struct CatTranDatabaseType> db_;

private:
  static constexpr unsigned int NUM_SAMPLES = 34;
};

#endif /* CATTRAN_DATABASE_H */
